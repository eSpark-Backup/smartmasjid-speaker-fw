#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "SM_Http.hpp"
#include "SM_Json.hpp"
#include "Device.hpp"
#include "BLE.hpp"
#include "Mqtt.hpp"

bool SmHttp::startAdhan() 
{
  if (WiFi.status() != WL_CONNECTED)
    return false;

  std::string httpsUrl = smHost;
  httpsUrl += adhanStartEndpoint;

  DynamicJsonDocument jsonDoc(64); 
  jsonDoc["masjidId"] = MASJID_ID_TEST;
  std::string jsonString;
  serializeJson(jsonDoc, jsonString);
  if (sendHttpsPostRequest(httpsUrl.c_str(),jsonString.c_str()))
  {
    return true;
  } 
  else 
  {
    return false;
  }
  return true;
}
bool SmHttp::sendDeviceTelemetry(const char* macAddress, uint8_t retryInterval, uint16_t maxRetryDuration)
{
  if (WiFi.status() != WL_CONNECTED)
    return false;

  unsigned long startTime = millis();
  while ((millis() - startTime) < TO_MS(maxRetryDuration)) 
  {
    if (sendDeviceTelemetry(macAddress)) 
    {
      return true;
    }
    Serial.println("Failed to send telemetry, retrying in 10 seconds...");
    delay(TO_MS(retryInterval));
  }
  return false;
}
bool SmHttp::sendDeviceTelemetry(const char* macAddress) 
{
  if (WiFi.status() != WL_CONNECTED)
    return false;

  std::string httpsUrl = smHost;
  httpsUrl += telemetryEndpoint;

  char telemetryJson[64];
  SM_Json::create(telemetryJson, sizeof(telemetryJson),
                  std::make_pair("battery", BATTERY_HEALTH),
                  std::make_pair("macAddress", Device::macAddress));
  Serial.println(telemetryJson);

  if (sendHttpsPostRequest(httpsUrl.c_str(), telemetryJson)) 
  {
    return true;
  } 
  else 
  {
    return false;
  }
}
bool SmHttp::stopAdhan() 
{
  if (WiFi.status() != WL_CONNECTED)
    return false;
  std::string httpsUrl = smHost;
  httpsUrl += adhanStopEndpoint;

  DynamicJsonDocument jsonDoc(64); 
  jsonDoc["masjidId"] = MASJID_ID_TEST;
  std::string jsonString;
  serializeJson(jsonDoc, jsonString);

  if (sendHttpsPostRequest(httpsUrl.c_str(),jsonString.c_str())) 
  {
    return true;
  } 
  else 
  {
    return false;
  }
  
}
bool SmHttp::sendHttpsPostRequest(const char* url, const char* json) 
{
  
  client->setInsecure();
  HTTPClient https;
  https.begin(*client, url);
  https.addHeader("Content-Type", "application/json");
  https.addHeader("x-api-key", EXTERNAL_API_KEY);
  
  Serial.println("Free heap memory: " + String(ESP.getFreeHeap()));
  delay(1000);
  if (strcmp(url,"https://smartmasjid-api-qa.futuriklabs.com/external/device/telemetry"))
  {
    
  }

  int httpCode = https.POST(json);
  const char* response = https.getString().c_str();

  https.end();
  if ((httpCode == HTTP_STATUS_OK) || (httpCode == HTTP_STATUS_CREATED)) 
  {
    Serial.println("Request successful!");
    Serial.println(response);
    return true;
  } 
  else 
  {
    Serial.print("Request failed with error code: ");
    Serial.println(httpCode);
    Serial.println(response);
    return false;
  }

}
