#pragma once
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WifiProvisioning.hpp"
#include "Device.hpp"
typedef enum 
{
  DEVICE_RESET,
  DEVICE_SUBSCRIBE,
  DEVICE_UNSUBSCRIBE,
  DEVICE_Wifi,
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
    WifiProvisioning* pWifiProv;
    // MQTT broker configuration
    const char* mqttServer = "smartmasjid-broker-dev.futuriklabs.com";
    const int mqttPort = 1883;
    const char* mqttUsername = "device";
    const char* mqttPassword = "device";

    /* MQTT topic details */
    const char* topicPrefix = "device";
    
    // MQTT topic to subscribe to
    const char* mqttTopicReset  = "reset";
    const char* mqttTopicWifi  = "wifi";
    

  public:
    MQTTClient(WifiProvisioning* wifiProv) : pWifiProv(wifiProv) {}
    void setup();
    void connectToBroker();
    void subscribeTopics();
    void unsubscribeTopics();
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
