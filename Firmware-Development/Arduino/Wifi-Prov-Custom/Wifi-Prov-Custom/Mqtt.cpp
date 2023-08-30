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
  while (retries--) 
  {
    if (connectToBroker()) 
    {
      return true;
    }
  }
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
      isConnected = true;  // Set the connection status
      return true;
    } 
    else 
    {
      Serial.println("Failed to connect to MQTT broker");
      isConnected = false;  // Set the connection status
      return false;
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

  switch (getTopic(topic)) 
  {
    case DEVICE_RESET:
      Serial.println("RESET TOPIC");
      unsubscribeTopics();
      Device::deleteMasjidId();
      Device::deleteWificredentials();
      break;

    case DEVICE_Wifi:
      {
        Serial.println("Wifi TOPIC");
        char ssid[35];
        char pass[35];
        SM_Json::parse((const char*)payload, std::make_pair("ssid", ssid), std::make_pair("password", pass));
        Device::updateWifiCredential(ssid, pass);
        if (pWifiProv->connectWifi(ssid, pass, 10)) 
        {
          // Setup MQTT again
          setup();
        } 
        else 
        {
          Serial.println("Restart ESP.....");
          ESP.restart();
        }
        break;
      }

    default:
      Serial.println("UNKNOWN TOPIC");
      break;
  }
}

void MQTTClient::loop() {
  if (!mqttClient.connect("arduino_client", mqttUsername, mqttPassword) && Connected) 
  {
    Connected = mqttCallback();
  }
  mqttClient.loop();
  isConnected = false;
}

bool MQTTClient::subscribe(const char* topic) 
{
  if (mqttClient.subscribe(topic)) 
  {
    Serial.printf("Subscribed to %s\n", topic);
    return true;
  }
  return false;
}

bool MQTTClient::unsubscribe(const char* topic) 
{
  if (mqttClient.unsubscribe(topic)) 
  {
    Serial.printf("Unsubscribed to %s\n", topic);
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

void MQTTClient::unsubscribeTopics() 
{
  char topicPath[50];
  getTopicPath(topicPath, mqttTopicReset);
  unsubscribe(topicPath);
  getTopicPath(topicPath, mqttTopicWifi);
  unsubscribe(topicPath);
}

bool MQTTClient::mqttCallback() 
{
  if (!isConnected) 
  {
    Serial.println("MQTT client not connected. Reconnecting...");
    Connected = connectToBroker(10);  // Reconnect
    return Connected;
  }
  return false;  // Not reconnected
}
