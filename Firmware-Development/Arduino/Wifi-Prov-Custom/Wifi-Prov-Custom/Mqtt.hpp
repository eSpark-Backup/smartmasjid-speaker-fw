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

class MQTTClient
{
  private:
    // MQTT broker configuration
    const char* mqtt_server = "smartmasjid-broker-dev.futuriklabs.com";
    const int mqtt_port = 1883;
    const char* mqtt_username = "device";
    const char* mqtt_password = "device";

    // MQTT topic to subscribe to
    const char* mqtt_topic_reset = "device/abc:123:efg:456/reset";
    const char* mqtt_topic_device_subscribe = "device/abc:123:efg:456/subscribe";

  public:
    void connectToBroker();
    void subscribeTopics();
    void receiveCallback(char* topic, byte* payload, unsigned int length);
    void loop();
    bool subscribe(const char* topic);
    uint8_t getTopic(const char* topic);
    bool extractTopicAndSubscribe(const char* message);
};
