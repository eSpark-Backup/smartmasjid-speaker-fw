#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "SM_Http.hpp"
#include "SM_Json.hpp"
#include "Device.hpp"

bool SmHttp::startAdhan()
{
  // Send the POST request
  HTTPClient http;
  http.begin(smHost, 80, adhanStartEndpoint);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("x-api-key", EXTERNAL_API_KEY);

  char masjidIdJson[64]; 
  SM_Json::create(masjidIdJson, sizeof(masjidIdJson),
                      std::pair<const char*, uint32_t>("masjidId", MASJID_ID_TEST));
  Serial.println(masjidIdJson);
  
  int httpCode = http.POST(masjidIdJson);
  const char* response = http.getString().c_str();

  http.end();

  if ((httpCode == HTTP_STATUS_OK) || (httpCode == HTTP_STATUS_CREATED)) 
  {
    Serial.println("Adhan Started!");
    Serial.println(response);
    return true;
  } 
  else 
  {
    Serial.print("Failed to start adhan with error code: ");
    Serial.println(httpCode);
    Serial.println(response);
    return false;
  }
}

bool SmHttp::sendDeviceTelemetry(const char* macAddress, uint8_t retryInterval, uint16_t maxRetryDuration) 
{
  // Start retry loop
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
  // Make HTTP POST request
  HTTPClient http;
  http.begin(smHost, 80, telemetryEndpoint);
  http.addHeader("x-api-key", EXTERNAL_API_KEY);
  http.addHeader("Content-Type", "application/json");
  
  char telemetryJson[64]; 
  SM_Json::create(telemetryJson, sizeof(telemetryJson),
                      std::pair<const char*, uint8_t>("battery", BATTERY_HEALTH),
                      std::pair<const char*, const char*>("macAddress", macAddress));
  Serial.println(telemetryJson);
  
  int httpCode = http.POST(telemetryJson);
  const char* response = http.getString().c_str();
  http.end();

  if ((httpCode == HTTP_STATUS_OK) || (httpCode == HTTP_STATUS_CREATED)) 
  {
    Serial.println("Telemetry sent successfully!");
    Serial.println(response);
    return true;
  } 
  else 
  {
    Serial.print("Failed to send telemetry with error code: ");
    Serial.println(httpCode);
    Serial.println(response);
    return false;
  }
}
