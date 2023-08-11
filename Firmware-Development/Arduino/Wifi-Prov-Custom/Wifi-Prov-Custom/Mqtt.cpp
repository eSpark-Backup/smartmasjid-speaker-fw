#include <ArduinoJson.h>
#include <Preferences.h>
#include "Mqtt.hpp"
#include "WifiProvisioning.hpp"
#include "SM_Json.hpp"
#include "SM_Nvs.hpp"

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void MQTTClient::setup()
{  

  connectToBroker();
  subscribeTopics();
}
void MQTTClient::connectToBroker() 
{
  mqttClient.setServer(mqttServer, mqttPort);
  while (!mqttClient.connected()) 
  {
    if (mqttClient.connect("arduino_client", mqttUsername, mqttPassword)) 
    {
      Serial.println("Connected to MQTT broker");
    } 
    else 
    {
      Serial.println("Failed to connect to MQTT broker");
      delay(1000);
    }
  }
}

void MQTTClient::subscribeTopics() 
{
  mqttClient.setCallback(std::bind(&MQTTClient::receiveCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

  char topicPath[50];
  getTopicPath(topicPath, mqttTopicReset);
  subscribe(topicPath);
  getTopicPath(topicPath, mqttTopicSubscribe);
  subscribe(topicPath);
  getTopicPath(topicPath, mqttTopicUnsubscribe);
  subscribe(topicPath);
  getTopicPath(topicPath, mqttTopicWifi);
  subscribe(topicPath);
  subscribeTopicsfromNVS();

}

void MQTTClient::receiveCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received MQTT message [");
  Serial.print(topic);
  Serial.print("]: ");

  Serial.write(payload, length);
  Serial.println();
  switch(getTopic(topic))
{
    case DEVICE_RESET:
      Serial.println("RESET TOPIC");
      unsubscribeTopics();
      if (Sm_Nvs::del(DEVICE_FACTORY_INFO_NAMESPACE, "masjidId"))
      {
        Serial.println("Masjid Id Removed");
      }
      if (Sm_Nvs::del(DEVICE_WIFI_CREDENTIALS_NAMESPACE, "ssid", "pass"))
      {
        Serial.println("Old credential removed");
      }
      break;

    case DEVICE_SUBSCRIBE:
      extractTopicAndSubscribe((const char*)payload);
      Serial.println("SUBSCRIBE TOPIC");
      break;

    case DEVICE_UNSUBSCRIBE:
      extractTopicAndUnsubscribe((const char*)payload);
      Serial.println("UNSUBSCRIBE TOPIC");
      break;

    case DEVICE_Wifi:  
      {   
        Serial.println("Wifi TOPIC");  
        if (Sm_Nvs::del(DEVICE_WIFI_CREDENTIALS_NAMESPACE, "ssid", "pass"))
        {
          Serial.println("Old credential removed");
        } 
           
        char ssid[32];
        char pass[32];
        if (SM_Json::parse((const char*)payload, std::make_pair("ssid", ssid), std::make_pair("password", pass))) 
        {
          // Store credentials in NVS
          if (Sm_Nvs::put(DEVICE_WIFI_CREDENTIALS_NAMESPACE,
                      std::make_pair("ssid", ssid),
                      std::make_pair("pass", pass)))
          {
            Serial.println("Wifi credentials updated");
          }
                      
          if (pWifiProv->connectWifi(ssid, pass, 10))
          {
            // Setup MQTT again
            setup();
          }

        }
        else
        {
          Serial.println("Unable to parse wifi credentials");
        }
      }

      break;
      
    default:
      Serial.println("UNKNOWN TOPIC");
      break;
  }
}


void MQTTClient::loop() 
{
  mqttClient.loop();
}

bool MQTTClient::subscribe(const char* topic)
{
  if (mqttClient.subscribe(topic))
  {
    Serial.printf("Subscribed to %s\n",topic);
    return true;
  }
  return false;
}

bool MQTTClient::unsubscribe(const char* topic)
{
  if (mqttClient.unsubscribe(topic))
  {
    Serial.printf("Unsubscribed to %s\n",topic);
    return true;
  }
  return false;
}

mqttTopic MQTTClient::getTopic(const char* topic)
{
  if (compareTopic(topic, mqttTopicReset))
  {
    return DEVICE_RESET;
  }

  else if (compareTopic(topic, mqttTopicSubscribe))
  {
    return DEVICE_SUBSCRIBE;
  }

  else if (compareTopic(topic, mqttTopicUnsubscribe))
  {
    return DEVICE_UNSUBSCRIBE;
  }
  else if (compareTopic(topic, mqttTopicWifi))
  {
    return DEVICE_Wifi;
  }
  
  return UNKNOWN_TOPIC;
}

bool MQTTClient::compareTopic(const char* topic, const char* topicName)
{
  char topicPath[50];
  getTopicPath(topicPath, topicName);
  return !strcmp(topicPath, topic);
}

bool MQTTClient::extractTopicAndSubscribe(const char* message) 
{

  return extractTopicAndControlSubscription(SUBSCRIBE, message);
}

bool MQTTClient::extractTopicAndUnsubscribe(const char* message) 
{

  return extractTopicAndControlSubscription(UNSUBSCRIBE, message);
}

bool MQTTClient::extractTopicAndControlSubscription(subscription control, const char* message) 
{
  bool ret = false;
  char topic[64];
  if (SM_Json::parse(message, std::make_pair("topic", topic))) 
  {
    ret = controlSubscription(control, topic); 
  }
  else 
  {
    Serial.println("Unable to parse topic");  
  }
  return ret;
}

bool MQTTClient::controlSubscription(subscription control, const char* topic)
{
  bool ret = false;
  switch (control)
  {
    case SUBSCRIBE:
      ret = subscribe(topic);
      break;
    
    case UNSUBSCRIBE:
      ret = unsubscribe(topic);
      break;
    
    default:
      Serial.println("UNKNOWN CONTROL");
  }
  return ret;
}

void MQTTClient::getTopicPath(char* dest, const char* topicName)
{
  // Copy the prefix to the destination buffer
  strcpy(dest, topicPrefix);

  // Add the MAC address to the destination buffer, separating it with slashes
  strcat(dest, "/");
  strcat(dest, Device::macAddress);

  // Add the topic name to the destination buffer, also separated with slashes
  strcat(dest, "/");
  strcat(dest, topicName);
}

void MQTTClient::subscribeTopicsfromNVS()
{
    NVS.begin("Subscription");
    uint32_t count = NVS.getUInt("count");
    for (int i =0 ; i<=count;i++)
    {
      String key_value = NVS.getString(String(i,DEC).c_str(),"");
      if (key_value =="")
        break;
      const char* topic = key_value.c_str();
      if (mqttClient.subscribe(topic))
        {
        Serial.printf("Subscribed to %s\n",topic);
        }

    }
}
void MQTTClient::unsubscribeTopicsfromNVS()
{
    NVS.begin("Subscription");
    uint32_t count = NVS.getUInt("count");
    for (int i =0 ; i<=count;i++)
    {
      String key_value = NVS.getString(String(i,DEC).c_str(),"");
      if (key_value =="")
        break;
      const char* topic = key_value.c_str();
      if (mqttClient.unsubscribe(topic))
        {
        Serial.printf("Unsubscribed to %s\n",topic);
        }

    }
}
void MQTTClient::removeMasjidIdfromNVS()
{
      NVS.begin("FactoryInfo");
      NVS.remove("masjid_id");
}

void MQTTClient::unsubscribeTopics()
{
  char topicPath[50];
  getTopicPath(topicPath, mqttTopicReset);
  unsubscribe(topicPath);
  getTopicPath(topicPath, mqttTopicSubscribe);
  unsubscribe(topicPath);
  getTopicPath(topicPath, mqttTopicUnsubscribe);
  unsubscribe(topicPath);
  getTopicPath(topicPath, mqttTopicWifi);
  unsubscribe(topicPath);
}
