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

HttpStatusCode SmHttp::sendDeviceTelemetry(const char* macAddress, uint8_t retryInterval, uint16_t maxRetry)
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Wifi not connected : Unable to send request");
    return false;
  }
  
  HttpStatusCode status;
  while (maxRetry--) 
  {
    status = sendDeviceTelemetry(macAddress);
    if (status == Http::Response::OK || status == Http::Response::CREATED)
    {
       break;
    }
    else
    {
      Serial.println("Failed to send telemetry, retrying in 10 seconds...");
      delay(TO_MS(retryInterval));
    }
  }
  return status;
}

HttpStatusCode SmHttp::sendDeviceTelemetry(const char* macAddress) 
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Wifi not connected : Unable to send request");
    return false;
  }
  
  std::string deviceTelemetryUrl = smHost;
  deviceTelemetryUrl += telemetryEndpoint;

  char telemetryJson[64];
  SM_Json::create(telemetryJson, sizeof(telemetryJson),
                  std::make_pair("battery", BATTERY_HEALTH),
                  std::make_pair("macAddress", Device::macAddress));
  Serial.println(telemetryJson);

  return post(deviceTelemetryUrl.c_str(),telemetryJson);
}

HttpStatusCode SmHttp::post(const char* url, const char* json) 
{
  
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient https;
  https.setReuse(false);
  https.begin(client, url);
  https.addHeader("Content-Type", "application/json");
  https.addHeader("x-api-key", EXTERNAL_API_KEY);
  HttpStatusCode status = https.POST(json);
  const char* response = https.getString().c_str();
  https.end();
  Serial.print("WIFI RSSI : ");
  Serial.println(WiFi.RSSI());
  if (status == Http::Response::OK || status == Http::Response::CREATED) 
  {
    Serial.println("Request successful!");
  } 
  else 
  {
    Serial.print("Request failed with error code: ");
    Serial.println(status);
  }
  Serial.println(response);
  return status;
}
