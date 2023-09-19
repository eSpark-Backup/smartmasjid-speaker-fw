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
  DEVICE_WIFI,
  DEVICE_ADHAN,
  UNKNOWN_TOPIC = 0xFF,
} mqttTopic;

typedef enum
{
  fajr = 1,
  dhuhr,
  asr,
  maghrib,
  isha,
  jummah1,
  jummah2,
  jummah3,
} Adhan;

typedef enum 
{
   SUBSCRIBE,
   UNSUBSCRIBE,
} subscription;

class MQTTClient
{
  private:
    IWifiProvisioning* pWifiProv;
    // MQTT broker configuration
    const char* mqttServer = "smartmasjid-broker-dev.futuriklabs.com";
    const int mqttPort = 1883;
    const char* mqttUsername = "device";
    const char* mqttPassword = "device";
    int count = 0 ;
    /* MQTT topic details */
    const char* topicPrefix = "device";
    char macAddress[20];
    
    // MQTT topic to subscribe to
    const char* mqttTopicReset  = "reset";
    const char* mqttTopicSubscribe  = "subscribe";
    const char* mqttTopicUnsubscribe  = "unsubscribe";
    const char* mqttTopicWifi  = "wifi";
    char adhan[32];
    bool Connected = true;
    bool isConnected = false;

  public:
    MQTTClient(IWifiProvisioning* wifiProv) : pWifiProv(wifiProv) {}
    bool setup();
    bool connectToBroker(int retries) ;
    bool connectToBroker();
    bool subscribeTopics();
    bool unsubscribeTopics();
    void receiveCallback(char* topic, byte* payload, unsigned int length);
    void loop();
    bool subscribe(const char* topic);
    bool unsubscribe(const char* topic);
    mqttTopic getTopic(const char* topic);
    bool extractTopicAndControlSubscription(subscription control, const char* message);
    bool extractTopicAndSubscribe(const char* message);
    bool extractTopicAndUnsubscribe(const char* message);
    void getTopicPath(char* dest, const char* topicName);
    bool getAdhan(const char* topic); 
    bool compareTopic(const char* topic, const char* topicName);
    bool controlSubscription(subscription control, const char* topic);
    bool subscribeTopicsfromNVS();
    bool unsubscribeTopicsfromNVS();
    bool mqttCallback();
    Adhan enumAdhan (char* adhan);
    void adhanStartOrStop(char* adhan,char*action);

};
