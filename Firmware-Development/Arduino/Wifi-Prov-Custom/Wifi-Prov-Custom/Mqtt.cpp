#include "Mqtt.hpp"

// MQTT broker configuration
const char* mqtt_server = "smartmasjid-broker-dev.futuriklabs.com";
const int mqtt_port = 1883;
const char* mqtt_username = "device";
const char* mqtt_password = "device";

// MQTT topic to subscribe to
const char* mqtt_topic_reset = "device/abc:123:efg:456/reset";
const char* mqtt_topic_device_subscribe = "device/abc:123:efg:456/subscribe";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void connectToMQTTBroker() {
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

void subscribeToMQTTTopic() {
  mqttClient.setCallback(mqttCallback);
  mqttTopicSubscribe(mqtt_topic_reset);
  mqttTopicSubscribe(mqtt_topic_device_subscribe);
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received MQTT message [");
  Serial.print(topic);
  Serial.print("]: ");

  Serial.write(payload, length);
  Serial.println();

  switch(getMqttTopic(topic))
  {
    case DEVICE_RESET:
      Serial.println("RESET TOPIC");
      break;
      
    case DEVICE_SUBSCRIBE:
      extractMqttTopicAndSubscribe((const char*)payload);
      Serial.println("SUBSCRIBE TOPIC");
      break;
      
    default:
      Serial.println("UNKNOWN TOPIC");
  }
}

void MQTTLoop() {
  mqttClient.loop();
}

bool mqttTopicSubscribe(const char* topic)
{
  if (mqttClient.subscribe(topic))
  {
    Serial.printf("Subscribed to %s\n",topic);
    return true;
  }
  return false;
}

uint8_t getMqttTopic(const char* topic)
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

bool extractMqttTopicAndSubscribe(const char* message) {
  DynamicJsonDocument doc(256);
  DeserializationError error = deserializeJson(doc, message);
  if (error) {
    Serial.println("Error parsing JSON");
  } 
  else 
  {
    const char* topic = doc["topic"];
    if (mqttTopicSubscribe(topic))
    {
      return true;
    }
  }
  return false;
}
