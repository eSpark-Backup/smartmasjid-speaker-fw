#include <ArduinoJson.h>
#include "Mqtt.hpp"
#include "WifiProvisioning.hpp"
#include "SM_Json.hpp"
#include "SM_Nvs.hpp"
#include "Device.hpp"
#include"Amplifier.hpp"

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

bool MQTTClient::setup() 
{
  mqttClient.setSocketTimeout (10);
  mqttClient.setKeepAlive (60);
  if (connectToBroker() && subscribeTopics())
    {
      return true; 
    }

  return false;
}

bool MQTTClient::connectToBroker(int retries) 
{
  while (retries--) 
  {
    if (connectToBroker())
    { 
      return true;
    }
     delay(timeDelay * 1000);
  }

  return false;
}

bool MQTTClient::connectToBroker() 
{
  mqttClient.setServer(mqttServer, mqttPort);
  while (!mqttClient.connected()) 
  {
    if (mqttClient.connect("arduino_client", mqttUsername, mqttPassword)) 
    {
      Serial.println("Connected to MQTT broker");
      isConnected = true; // Set the connection status
      return true;
    } 
    else
     {
      Serial.println("Failed to connect to MQTT broker");
      isConnected = false; // Set the connection status
      return false;
    }
  }
}

bool MQTTClient::subscribeTopics()
{
  mqttClient.setCallback(std::bind(&MQTTClient::receiveCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

  char topicPath[50];

  const char* topicNames[] = 
  {
    mqttTopicReset,
    mqttTopicSubscribe,
    mqttTopicUnsubscribe,
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

  if(!subscribeTopicsfromNVS())
      return false;

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
      Device::deleteWificredentials();
      break;

    case DEVICE_SUBSCRIBE:
      extractTopicAndSubscribe((const char*)payload);
      Serial.println("SUBSCRIBE TOPIC");
      break;

    case DEVICE_UNSUBSCRIBE:
      extractTopicAndUnsubscribe((const char*)payload);
      Serial.println("UNSUBSCRIBE TOPIC");
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
        };
        break;
      }
      case DEVICE_ADHAN:
      {
        Amplifier amplifier;
        char action[50];
        SM_Json::parse((const char*)payload,
                       std::make_pair("action", action));

        amplifier.adhanStream(adhan,action);
        break;
      }

    default:
      Serial.println("UNKNOWN TOPIC");
      break;
  }
}

void MQTTClient::loop() 
{
  if (!mqttClient.connect("arduino_client", mqttUsername, mqttPassword) && Connected) 
  {
    Connected = mqttCallback();
  }

  mqttClient.loop();
  isConnected = false;
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

  else if (compareTopic(topic, mqttTopicSubscribe)) 
  {
    return DEVICE_SUBSCRIBE;
  }

  else if (compareTopic(topic, mqttTopicUnsubscribe)) 
  {
    return DEVICE_UNSUBSCRIBE;
  } 
  else if (compareTopic(topic, mqttTopicWifi)) 
  {
    return DEVICE_WIFI;
  }
  else if (getAdhan(topic)) 
  {
    return DEVICE_ADHAN;
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
      {
        ret = subscribe(topic);
        Device::storeSubscription(topic);
        break;
      }

    case UNSUBSCRIBE:
      {
        ret = unsubscribe(topic);
        Device::deleteSubscription(topic);
        break;
      }

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

bool MQTTClient::subscribeTopicsfromNVS() 
{
  uint32_t count;
  if (!Sm_Nvs::get(DEVICE_SUBSCRIPTION_NAMESPACE, std::make_pair("count", &count))) 
  {
    // Error getting count from NVS
    Serial.println("Error getting count from NVS");
    return false;
  }
   int refCounter = 0; 
  for (int i = 0; i < count; i++) 
  {
    char keyValue[50];
    if (!Sm_Nvs::get(DEVICE_SUBSCRIPTION_NAMESPACE, std::make_pair(String(i, DEC).c_str(), keyValue))) 
    {
      // Error getting topic from NVS
      Serial.println("Error getting topic from NVS");
      Serial.println(count);
      if (refCounter <count)
      {
       continue;  // Move to the next iteration
      }
      else
         break;
    }

    if (keyValue[0] == '\0') 
    {
      // Empty topic, skip
      Serial.println("Empty topic, skip...");
      continue;
    }

    const char* topic = keyValue;

    if (mqttClient.subscribe(topic))
    {
      Serial.printf("Subscribed to %s\n", topic);
    } 
    else 
    {
      Serial.printf("Failed to subscribe to %s\n", topic);
      return false;  // If any subscription fails, return false
    }
  }

  return true;  // If all subscriptions succeed
}

bool MQTTClient::unsubscribeTopicsfromNVS() 
{
  uint32_t count;
  if(!Sm_Nvs::get(DEVICE_SUBSCRIPTION_NAMESPACE, std::make_pair("count", &count)))
      {
        Serial.println("Error getting count from NVS");
        return false;
      }
  
  for (int i = 0; i <= count; i++) 
  {
    char keyValue[50];
    if(!Sm_Nvs::get(DEVICE_SUBSCRIPTION_NAMESPACE, std::make_pair(String(i, DEC).c_str(), keyValue)))
       {
        Serial.println("Error getting topic from NVS");
        return false;
       }      

    if (keyValue[0] == '\0')
      break;

    const char* topic = keyValue;

    if (!mqttClient.unsubscribe(topic)) 
    {
      Serial.printf("Failed to unsubscribe from %s\n", topic);
       return false;
    }
    else
    {
      Serial.printf("Unsubscribed from %s\n", topic);
    }
  }
  return true;
}

bool MQTTClient::unsubscribeTopics() 
{
  char topicPath[50];
  const char* topicNames[] = {
    mqttTopicReset,
    mqttTopicSubscribe,
    mqttTopicUnsubscribe,
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

  if (!unsubscribeTopicsfromNVS())
     return false;

  return true;
}

bool MQTTClient::mqttCallback() 
{
  if (!isConnected) 
  {
      Serial.println("MQTT client not connected. Reconnecting...");
      mqttState();
      Connected = connectToBroker(10); // Reconnect
      return Connected;
  }
  return false; // Not reconnected
}

bool MQTTClient::getAdhan(const char* topic) 
{
  memset(adhan, 0, sizeof(adhan));
  char* nonConstTopic = const_cast<char*>(topic);
  char* token = strtok(nonConstTopic, "/");
  int tokenCount = 0;

  while (token != NULL) 
  {
    switch (tokenCount) 
    {
      case 3:
        strncpy(adhan, token, sizeof(adhan)+2);
        return true ;
        break;
    }
    token = strtok(NULL, "/");
    tokenCount++;
  }
  Serial.println ("unsuccessful");
  return false;
}

void MQTTClient::mqttState()
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
