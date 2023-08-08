#include "Mqtt.hpp"
#include "WifiProvisioning.hpp"
#include <ArduinoJson.h>
#include <Preferences.h>
Preferences NVS;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void MQTTClient::setup()
{  
  getDeviceMAC(macAddress);
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
    } else 
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
  getTopicPath(topicPath, mqttTopicWifi);
  subscribe(topicPath);
}

void MQTTClient::receiveCallback(char* topic, byte* payload, unsigned int length) 
{
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
      removeMasjidIdfromNVS();
      if (pWifiProv->removeWifiCredsNVS())
      {
        Serial.println("Old Credential removed");
      }
      break;

    case DEVICE_Wifi:     
      Serial.println("Wifi TOPIC");     
      if (pWifiProv->removeWifiCredsNVS())
      {
        Serial.println("Old Credential removed");
      }     
      char ssid[32];
      char pass[32];
      parseCredentials(ssid, pass, (const char*)payload, length);
      pWifiProv->storeWifiCredsNVS(ssid, pass);
      pWifiProv->connectWifi(ssid, pass, 10);
      break;
      
    default:
      Serial.println("UNKNOWN TOPIC");
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
  
  DynamicJsonDocument doc(256);
  DeserializationError error = deserializeJson(doc, message);
  if (error) 
  {
    Serial.println("Error parsing JSON");
  } 
  else 
  {
    const char* topic = doc["topic"];
    ret = controlSubscription(control, topic);  
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
  strcat(dest, macAddress);

  // Add the topic name to the destination buffer, also separated with slashes
  strcat(dest, "/");
  strcat(dest, topicName);
}

bool MQTTClient::parseCredentials(char* pSsid, char* pPass, const char* jsonpacket, unsigned int length) 
{
  StaticJsonDocument<125> doc;
  DeserializationError error = deserializeJson(doc, jsonpacket, length);
  if (error) 
  {
    Serial.println("Error parsing JSON");
    return false;
  } 
  
  const char* ssid = doc["ssid"];
  const char* pass = doc["password"];
  memcpy(pSsid, ssid , strlen(ssid)+1);
  memcpy(pPass, pass , strlen(pass)+1);
  
  return true;
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
  getTopicPath(topicPath, mqttTopicWifi);
  unsubscribe(topicPath);
}
