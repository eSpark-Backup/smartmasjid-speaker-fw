#pragma once
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

enum mqttTopics
{
  DEVICE_RESET,
  DEVICE_SUBSCRIBE,
  UNKNOWN_TOPIC = 0xFF,
};

void connectToMQTTBroker();
void subscribeToMQTTTopic();
void mqttCallback(char* topic, byte* payload, unsigned int length);
void MQTTLoop();
bool mqttTopicSubscribe(const char* topic);
uint8_t getMqttTopic(const char* topic);
bool extractMqttTopicAndSubscribe(const char* message);
