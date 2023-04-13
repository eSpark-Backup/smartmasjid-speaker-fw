#include "Mqtt.hpp"

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void MQTTClient::connectToBroker() {
  mqttClient.setServer(mqttServer, mqttPort);
  while (!mqttClient.connected()) {
    if (mqttClient.connect("arduino_client", mqttUsername, mqttPassword)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.println("Failed to connect to MQTT broker");
      delay(1000);
    }
  }
}

void MQTTClient::subscribeTopics() {
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
      break;
      
    case DEVICE_SUBSCRIBE:
      extractTopicAndSubscribe((const char*)payload);
      Serial.println("SUBSCRIBE TOPIC");
      break;

    case DEVICE_UNSUBSCRIBE:
      extractTopicAndUnsubscribe((const char*)payload);
      Serial.println("UNSUBSCRIBE TOPIC");
      break;
      
    default:
      Serial.println("UNKNOWN TOPIC");
  }
}

void MQTTClient::loop() {
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

uint8_t MQTTClient::getTopic(const char* topic)
{
  char topicPath[50];
  getTopicPath(topicPath, mqttTopicReset);
  if (!strcmp(topic, topicPath))
  {
    return DEVICE_RESET;
  }

  getTopicPath(topicPath, mqttTopicSubscribe);
  if (!strcmp(topic, topicPath))
  {
    return DEVICE_SUBSCRIBE;
  }

  getTopicPath(topicPath, mqttTopicUnsubscribe);
  if (!strcmp(topic, topicPath))
  {
    return DEVICE_UNSUBSCRIBE;
  }
  
  return UNKNOWN_TOPIC;
}

bool MQTTClient::extractTopicAndSubscribe(const char* message) {

  return extractTopicAndControlSubscription(SUBSCRIBE, message);
}

bool MQTTClient::extractTopicAndUnsubscribe(const char* message) {

  return extractTopicAndControlSubscription(UNSUBSCRIBE, message);
}

bool MQTTClient::extractTopicAndControlSubscription(subscription control, const char* message) {
  DynamicJsonDocument doc(256);
  DeserializationError error = deserializeJson(doc, message);
  if (error) {
    Serial.println("Error parsing JSON");
  } 
  else 
  {
    const char* topic = doc["topic"];
    switch (control)
    {
      case SUBSCRIBE:
      if (subscribe(topic))
      {
        return true;
      }
      break;
      
      case UNSUBSCRIBE:
      if (unsubscribe(topic))
      {
        return true;
      }
      break;
      
      default:
      Serial.println("UNKNOWN CONTROL");
    }
    
  }
  return false;
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
