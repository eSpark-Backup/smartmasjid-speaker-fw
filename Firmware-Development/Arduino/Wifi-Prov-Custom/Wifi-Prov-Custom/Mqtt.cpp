#include <ArduinoJson.h>
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
bool MQTTClient::connectToBroker(int retries) 
{
  do
  {
    if(connectToBroker())
    {
      return true;
    }
  } while (--retries);
  return false;
}
bool MQTTClient::connectToBroker() 
{
  mqttClient.setServer(mqttServer, mqttPort);
  while (!mqttClient.connected()) 
  {
    if (mqttClient.connect("arduino_client", mqttUsername, mqttPassword)) 
    {
      Serial.println("Connected to MQTT broker");
      return true;
    } 
    else 
    {
      Serial.println("Failed to connect to MQTT broker");
      return false;
    }
  }
}
bool MQTTClient::subscribeTopics() 
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

  return true;
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
        bool connectedtonewCredentials = false; 
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
            connectedtonewCredentials = true;
            Serial.println(connectedtonewCredentials);
          }
          if(!connectedtonewCredentials)
          {
            Serial.println("Restart ESP.....");
            delay(10);
            ESP.restart();
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
  
  if (!mqttClient.connected() && Connected) 
  {
    Serial.println("MQTT client not connected. Reconnecting...");
    Connected = connectToBroker(10);
  }
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
    { 
      ret = subscribe(topic);
      uint32_t c =0;
      Sm_Nvs::get(DEVICE_SUBSCRIPTION_NAMESPACE,
                  std::make_pair("count",&c)); 
      Sm_Nvs::put(DEVICE_SUBSCRIPTION_NAMESPACE,
                  std::make_pair(String(c, DEC).c_str(), topic));
      c++;
      Sm_Nvs::put(DEVICE_SUBSCRIPTION_NAMESPACE,
                  std::make_pair("count", c)); // Put the incremented value of c
      break;
    }
    
    case UNSUBSCRIBE:
    {     
    ret = unsubscribe(topic);
    uint32_t count ;
    Sm_Nvs::get(DEVICE_SUBSCRIPTION_NAMESPACE,
                    std::make_pair("count",&count));   
      bool found = false;
      for(int i =0 ; i<=count;i++)
    {
    char keyValue[50];
    Sm_Nvs::get(DEVICE_SUBSCRIPTION_NAMESPACE,
                  std::make_pair(String(i, DEC).c_str(), keyValue));

      if (strcmp(keyValue,topic)==0)
      {
        Sm_Nvs::del(DEVICE_SUBSCRIPTION_NAMESPACE, String(i,DEC).c_str());

        Sm_Nvs::get(DEVICE_SUBSCRIPTION_NAMESPACE,
                  std::make_pair(String(count-1, DEC).c_str(), keyValue)); 

        Sm_Nvs::put(DEVICE_SUBSCRIPTION_NAMESPACE,
                  std::make_pair(String(i, DEC).c_str(), keyValue)); 

        count--;

        Sm_Nvs::put(DEVICE_SUBSCRIPTION_NAMESPACE,
                    std::make_pair("count",count));   
                    
        found = true;
        break;
      }

      } 
        break;
    }
    
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
    uint32_t count ;
    Sm_Nvs::get(DEVICE_SUBSCRIPTION_NAMESPACE,
                    std::make_pair("count",&count)); 
    for (int i =0 ; i<=count;i++)
    {
     char keyValue[50]; 

     Sm_Nvs::get(DEVICE_SUBSCRIPTION_NAMESPACE,
                  std::make_pair(String(i,DEC).c_str(), keyValue));      
    
    if (keyValue[0] == '\0')
    break;
    
    const char* topic = keyValue;
    if (mqttClient.subscribe(topic))
    {
      Serial.printf("Subscribed to %s\n",topic);
    }

  }
}
bool MQTTClient::unsubscribeTopicsfromNVS()
{
    uint32_t count ;
    Sm_Nvs::get(DEVICE_SUBSCRIPTION_NAMESPACE,
                    std::make_pair("count",&count)); 
    for (int i =0 ; i<=count;i++)
    {
     char keyValue[50]; 

     Sm_Nvs::get(DEVICE_SUBSCRIPTION_NAMESPACE,
                  std::make_pair(String(i,DEC).c_str(), keyValue));      
    
    if (keyValue[0] == '\0')
    break;

    const char* topic = keyValue;
    if (mqttClient.unsubscribe(topic))
      {
        Serial.printf("Unsubscribed  to %s\n",topic);
        return true;
      }
    }
      return false;
}
bool MQTTClient::unsubscribeTopics()
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
  return true;
}
