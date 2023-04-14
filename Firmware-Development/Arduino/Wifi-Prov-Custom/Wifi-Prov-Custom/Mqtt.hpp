#pragma once
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

typedef enum 
{
  DEVICE_RESET,
  DEVICE_SUBSCRIBE,
  DEVICE_UNSUBSCRIBE,
  UNKNOWN_TOPIC = 0xFF,
} mqttTopic;

typedef enum 
{
   SUBSCRIBE,
   UNSUBSCRIBE,
} subscription;

class MQTTClient
{
  private:
    // MQTT broker configuration
    const char* mqttServer = "smartmasjid-broker-dev.futuriklabs.com";
    const int mqttPort = 1883;
    const char* mqttUsername = "device";
    const char* mqttPassword = "device";

    /* MQTT topic details */
    const char* topicPrefix = "device";
    const char* macAddress = "abc:123:efg:456";
    
    // MQTT topic to subscribe to
    const char* mqttTopicReset  = "reset";
    const char* mqttTopicSubscribe  = "subscribe";
    const char* mqttTopicUnsubscribe  = "unsubscribe";
    const char* mqttTopicWifi  = "wifi";

  public:
    void connectToBroker();
    void subscribeTopics();
    void receiveCallback(char* topic, byte* payload, unsigned int length);
    void loop();
    bool subscribe(const char* topic);
    bool unsubscribe(const char* topic);
    mqttTopic getTopic(const char* topic);
    bool extractTopicAndControlSubscription(subscription control, const char* message);
    bool extractTopicAndSubscribe(const char* message);
    bool extractTopicAndUnsubscribe(const char* message);
    void getTopicPath(char* dest, const char* topicName); 
    bool compareTopic(const char* topic, const char* topicName);
    bool controlSubscription(subscription control, const char* topic);
};
