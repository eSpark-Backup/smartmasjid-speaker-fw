#pragma once
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WifiProvisioning.hpp"
#include "Device.hpp"

#define TEXTIFY(A) #A
#define MQTT(State)    "MQTT state : " TEXTIFY(State)

namespace MQTT
{
  typedef enum
  {
    CONNECTION_TIMEOUT = -4,
    CONNECTION_LOST,
    CONNECTION_FAILED,
    DISCONNECTED,
    CONNECTED,
    CONNECT_BAD_PROTOCOL,
    CONNECT_BAD_CLIENT_ID,
    CONNECT_UNAVAILIABLE,
    CONNECT_BAD_CREDENTIALS,
    CONNECT_UNAUTHORIZED,
  }State;
}

typedef enum {
  DEVICE_RESET,
  DEVICE_SUBSCRIBE,
  DEVICE_UNSUBSCRIBE,
  DEVICE_Wifi,
  UNKNOWN_TOPIC = 0xFF,
} mqttTopic;

typedef enum {
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

  /* MQTT topic details */
  const char* topicPrefix = "device";

  // MQTT topic to subscribe to
  const char* mqttTopicReset = "reset";
  const char* mqttTopicWifi = "wifi";
  bool Connected = true;
  bool isConnected = false;



public:
  MQTTClient(IWifiProvisioning* wifiProv): pWifiProv(wifiProv) {}
  void setup();
  bool connectToBroker();
  bool connectToBroker(int retries);
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
  bool mqttCallback();
  void printState();
};
