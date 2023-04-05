#pragma once
#include <WiFi.h>
#include <PubSubClient.h>

void connectToMQTTBroker();
void subscribeToMQTTTopic();
void mqttCallback(char* topic, byte* payload, unsigned int length);
void MQTTLoop();
