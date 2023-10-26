#include <ArduinoJson.h>
#include "Mqtt.hpp"
#include "WifiProvisioning.hpp"
#include "SM_Json.hpp"
#include "SM_Nvs.hpp"

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

bool MQTTClient::setup() 
{
  if (connectToBroker(5))
  {
    return true; 
  }

  return false;
}

bool MQTTClient::connectToBroker(int retries) 
{
  while (retries--) 
  {
    if (connectToBroker() && subscribeTopics())
    { 
      return true;
    }
     delay(MQTT_RECONNECT_DELAY * 1000);
  }

  return false;
}

bool MQTTClient::connectToBroker() 
{
  mqttClient.setServer(mqttServer, mqttPort);
  static std::string clientId = "\0";
  if (clientId == "\0")
  {
    clientId = std::to_string(random(1000000000));
    Serial.print("Client id : ");
    Serial.println(clientId.c_str());
  }
    if (mqttClient.connect(clientId.c_str(), mqttUsername, mqttPassword)) 
    {
      Serial.println("Connected to MQTT broker");
      return true;
    } 
    else
     {
      Serial.println("Failed to connect to MQTT broker");
      return false;
    }
}


bool MQTTClient::subscribeTopics() 
{
  mqttClient.setCallback(std::bind(&MQTTClient::receiveCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

  char topicPath[50];

  const char* topicNames[] = 
  {
    mqttTopicReset,
    mqttTopicWifi
  };
  
  for (const char* topic : topicNames) 
  {
    getTopicPath(topicPath, topic);
    if (!subscribe(topicPath)) 
    {
      return false;
    }
  }

  return true;
}

void MQTTClient::receiveCallback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Received MQTT message [");
  Serial.print(topic);
  Serial.print("]: ");

  Serial.write(payload, length);
  Serial.println();

  switch (getTopic(topic)) 
  {
    case DEVICE_RESET:
      Serial.println("RESET TOPIC");
      unsubscribeTopics();
      Device::deleteMasjidId();
      Device::deleteWificredentials();
      ESP.restart();
      break;

    case DEVICE_WIFI:
      {
        Serial.println("Wifi TOPIC");
        char ssid[35];
        char pass[35];
        SM_Json::parse((const char*)payload, std::make_pair("ssid", ssid), std::make_pair("password", pass));
        Device::updateWifiCredential(ssid, pass);
        if (pWifiProv->connectWifi(ssid, pass, 10)) 
        {
          // Setup MQTT again
          setup();
        } 
        else 
        {
          Serial.println("Restart ESP.....");
          ESP.restart();
        }
        break;
      }

    default:
      Serial.println("UNKNOWN TOPIC");
      break;
  }
}

void MQTTClient::loop() 
{
  if (!mqttClient.connected()) 
      mqttCallback();

  mqttClient.loop();
}

bool MQTTClient::subscribe(const char* topic) 
{
  if (mqttClient.subscribe(topic)) 
  {
    Serial.printf("Subscribed to %s\n", topic);
    return true;
  }
  return false;
}

bool MQTTClient::unsubscribe(const char* topic) 
{
  if (mqttClient.unsubscribe(topic)) 
  {
    Serial.printf("Unsubscribed to %s\n", topic);
    return true;
  }
  return false;
}

mqttTopic MQTTClient::getTopic(const char* topic) 
{
  if (compareTopic(topic, mqttTopicReset)) 
  {
    return DEVICE_RESET;
  }

  else if (compareTopic(topic, mqttTopicWifi)) 
  {
    return DEVICE_WIFI;
  }
  
  return UNKNOWN_TOPIC;
}

bool MQTTClient::compareTopic(const char* topic, const char* topicName) 
{
  char topicPath[50];
  getTopicPath(topicPath, topicName);
  return !strcmp(topicPath, topic);
}

bool MQTTClient::extractTopicAndSubscribe(const char* message) 
{
  return extractTopicAndControlSubscription(SUBSCRIBE, message);
}

bool MQTTClient::extractTopicAndUnsubscribe(const char* message) 
{
  return extractTopicAndControlSubscription(UNSUBSCRIBE, message);
}

bool MQTTClient::extractTopicAndControlSubscription(subscription control, const char* message) 
{
  bool ret = false;
  char topic[64];
  if (SM_Json::parse(message, std::make_pair("topic", topic))) 
  {
    ret = controlSubscription(control, topic);
  } 
  else 
  {
    Serial.println("Unable to parse topic");
  }
  return ret;
}

bool MQTTClient::controlSubscription(subscription control, const char* topic) 
{
  bool ret = false;

  switch (control) 
  {
    case SUBSCRIBE:
      ret = subscribe(topic);
      break;

    case UNSUBSCRIBE:
      ret = unsubscribe(topic);
      break;

    default:
      Serial.println("UNKNOWN CONTROL");
  }

  return ret;
}

void MQTTClient::getTopicPath(char* dest, const char* topicName) 
{
  // Copy the prefix to the destination buffer
  strcpy(dest, topicPrefix);

  // Add the MAC address to the destination buffer, separating it with slashes
  strcat(dest, "/");
  strcat(dest, Device::macAddress);

  // Add the topic name to the destination buffer, also separated with slashes
  strcat(dest, "/");
  strcat(dest, topicName);
}

bool MQTTClient::unsubscribeTopics() 
{
  char topicPath[50];
  
  const char* topicNames[] = 
  {
    mqttTopicReset,
    mqttTopicWifi
  };

  for (const char* topic : topicNames) 
  {
    getTopicPath(topicPath, topic);
    if (!unsubscribe(topicPath)) 
    {
      return false;
    }
  }
  
  return true;
}

void MQTTClient::mqttCallback() 
{
    Serial.println("MQTT client not connected. Reconnecting...");
    printState();
    connectToBroker(10);
}

void MQTTClient::printState()
{
  int state = mqttClient.state();
  switch(state)
  {
    case MQTT::CONNECTION_TIMEOUT:
      printf("%s\n", MQTT(CONNECTION_TIMEOUT));
      break;

    case MQTT::CONNECTION_LOST:
      printf("%s\n", MQTT(CONNECTION_LOST));
      break;

    case MQTT::CONNECTION_FAILED:
      printf("%s\n", MQTT(CONNECTION_FAILED));
      break;

    case MQTT::DISCONNECTED:
      printf("%s\n", MQTT(DISCONNECTED));
      break;

    case MQTT::CONNECTED:
      printf("%s\n", MQTT(CONNECTED));
      break;

    case MQTT::CONNECT_BAD_PROTOCOL:
      printf("%s\n", MQTT(CONNECT_BAD_PROTOCOL));
      break;

    case MQTT::CONNECT_BAD_CLIENT_ID:
      printf("%s\n", MQTT(CONNECT_BAD_CLIENT_ID));
      break;

    case MQTT::CONNECT_UNAVAILIABLE:
      printf("%s\n", MQTT(CONNECT_UNAVAILIABLE));
      break;

    case MQTT::CONNECT_BAD_CREDENTIALS:
      printf("%s\n", MQTT(CONNECT_BAD_CREDENTIALS));
      break;

    case MQTT::CONNECT_UNAUTHORIZED:
      printf("%s\n", MQTT(CONNECT_UNAUTHORIZED));
      break;
  }
}

void MQTTClient::createTask()
{
 xTaskCreatePinnedToCore (mqttTask, "mqttTask", 10000, this, 0, NULL, 0);
}

void mqttTask(void* pvParameters) 
{
  MQTTClient* mqtt = (MQTTClient*) pvParameters;
  mqtt->setup();
  while(1)
  {
    mqtt->loop();
  }
  vTaskDelete( NULL );
}
