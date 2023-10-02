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

httpcode SmHttp::sendDeviceTelemetry(const char* macAddress, uint8_t retryInterval, uint16_t maxRetry)
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Wifi not connected : Unable to send request");
    return false;
  }
  
  int httpCode;
  while (maxRetry--) 
  {
    httpCode = sendDeviceTelemetry(macAddress);
    if ((httpCode == HttpStatus::OK) || (httpCode == HttpStatus::CREATED)) 
    {
       break;
    }
    else
    {
      Serial.println("Failed to send telemetry, retrying in 10 seconds...");
      delay(TO_MS(retryInterval));
    }
  }
  return httpCode;
}

httpcode SmHttp::sendDeviceTelemetry(const char* macAddress) 
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Wifi not connected : Unable to send request");
    return false;
  }
  
  std::string httpsUrl = smHost;
  httpsUrl += telemetryEndpoint;

  char telemetryJson[64];
  SM_Json::create(telemetryJson, sizeof(telemetryJson),
                  std::make_pair("battery", BATTERY_HEALTH),
                  std::make_pair("macAddress", Device::macAddress));
  Serial.println(telemetryJson);

  return post(httpsUrl.c_str(),telemetryJson);
}

httpcode SmHttp::post(const char* url, const char* json) 
{
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient https;
  https.begin(client, url);
  https.addHeader("Content-Type", "application/json");
  https.addHeader("x-api-key", EXTERNAL_API_KEY);
  int httpCode = https.POST(json);
  const char* response = https.getString().c_str();
  https.end();

  if ((httpCode == HTTP_STATUS_OK) || (httpCode == HTTP_STATUS_CREATED)) 
  {
    Serial.println("Request successful!");
  } 
  else 
  {
    Serial.print("Request failed with error code: ");
    Serial.println(httpCode);
  }
  
  Serial.println(response);
  return httpCode;
}
