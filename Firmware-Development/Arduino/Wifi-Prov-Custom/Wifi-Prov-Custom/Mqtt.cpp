#include "Mqtt.hpp"

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void MQTTClient::connectToBroker() {
  mqttClient.setServer(mqtt_server, mqtt_port);
  while (!mqttClient.connected()) {
    if (mqttClient.connect("arduino_client", mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.println("Failed to connect to MQTT broker");
      delay(1000);
    }
  }
}

void MQTTClient::subscribeTopics() {
  mqttClient.setCallback(std::bind(&MQTTClient::receiveCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  subscribe(mqtt_topic_reset);
  subscribe(mqtt_topic_device_subscribe);
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

uint8_t MQTTClient::getTopic(const char* topic)
{
  if (!strcmp(topic, mqtt_topic_reset))
  {
    return DEVICE_RESET;
  }
  else if (!strcmp(topic, mqtt_topic_device_subscribe))
  {
    return DEVICE_SUBSCRIBE;
  }
  return UNKNOWN_TOPIC;
}

bool MQTTClient::extractTopicAndSubscribe(const char* message) {
  DynamicJsonDocument doc(256);
  DeserializationError error = deserializeJson(doc, message);
  if (error) {
    Serial.println("Error parsing JSON");
  } 
  else 
  {
    const char* topic = doc["topic"];
    if (subscribe(topic))
    {
      return true;
    }
  }
  return false;
}
