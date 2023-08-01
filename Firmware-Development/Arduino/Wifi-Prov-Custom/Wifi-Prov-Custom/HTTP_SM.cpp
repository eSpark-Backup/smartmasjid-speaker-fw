#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "HTTP_SM.hpp"
#include "SM_Json.hpp"
#include "Device.h"

bool SmHttp::startAdhan()
{
  StaticJsonDocument<200> doc;
  doc["masjidId"] = 3; // For Testing
  String masjidIdJson;
  serializeJson(doc, masjidIdJson);

  // Send the POST request
  HTTPClient http;
  http.begin(smHost, 80, adhanStartEndpoint);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("x-api-key", EXTERNAL_API_KEY);
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
  
  SM_Json smJson;
  char telemetryJson[64]; 
  smJson.createTelemetryJson(macAddress, BATTERY_HEALTH, telemetryJson, sizeof(telemetryJson));
  
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
