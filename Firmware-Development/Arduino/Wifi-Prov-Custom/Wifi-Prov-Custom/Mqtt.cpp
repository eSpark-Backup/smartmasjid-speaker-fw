#include "Mqtt.hpp"

// MQTT broker configuration
const char* mqtt_server = "smartmasjid-broker-dev.futuriklabs.com";
const int mqtt_port = 1883;
const char* mqtt_username = "device";
const char* mqtt_password = "device";

// MQTT topic to subscribe to
const char* mqtt_topic_reset = "device/abc:123:efg:456/reset";

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
  mqttClient.subscribe(mqtt_topic_reset);
  Serial.println("Subscribed to MQTT topics");
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received MQTT message [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void MQTTLoop() {
  mqttClient.loop();
}
