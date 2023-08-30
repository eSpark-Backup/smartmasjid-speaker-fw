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

httpcode SmHttp::startAdhan() 
{
  if (WiFi.status() != WL_CONNECTED)
    return false;

  std::string httpsUrl = smHost;
  httpsUrl += adhanStartEndpoint;

  std::string jsonString = SM_Json::create("masjidId", &MASJID_TEST_ID);

  int httpCode = Post(httpsUrl.c_str(),jsonString.c_str());
    if (httpCode == HTTP_OK) 
      return HTTP_OK;
    else if(httpCode == HTTP_404_ERROR) 
      return HTTP_404_ERROR;
    else
      return httpCode;
}

httpcode SmHttp::sendDeviceTelemetry(const char* macAddress, uint8_t retryInterval, uint16_t maxRetry)
{
  if (WiFi.status() != WL_CONNECTED)
    return false;

  int Httpcode;
  while (maxRetry --) 
  {
    int httpCode = sendDeviceTelemetry(macAddress);
    if (httpCode == HTTP_OK) 
       return HTTP_OK;
    else
    {
      Serial.println("Failed to send telemetry, retrying in 10 seconds...");
      delay(TO_MS(retryInterval));
      Httpcode = httpCode;
    }
  }

  if(Httpcode == HTTP_404_ERROR) 
      return HTTP_404_ERROR;

  return Httpcode;
}

httpcode SmHttp::sendDeviceTelemetry(const char* macAddress) 
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

  int httpCode = Post(httpsUrl.c_str(),telemetryJson);
  if (httpCode == HTTP_OK) 
    return HTTP_OK;
  else if(httpCode == HTTP_404_ERROR) 
    return HTTP_404_ERROR;
  else
    return httpCode;
}

httpcode SmHttp::stopAdhan() 
{
  if (WiFi.status() != WL_CONNECTED)
    return false;

  std::string httpsUrl = smHost;
  httpsUrl += adhanStopEndpoint;

  std::string jsonString = SM_Json::create("masjidId", &MASJID_TEST_ID);
  Serial.println(jsonString.c_str());

  int httpCode = Post(httpsUrl.c_str(),jsonString.c_str());
  if (httpCode == HTTP_OK) 
      return HTTP_OK;
  else if(httpCode == HTTP_404_ERROR) 
      return HTTP_404_ERROR;
  else
      return httpCode;
  
}

httpcode SmHttp::Post(const char* url, const char* json) 
{
  WiFiClientSecure* client = new WiFiClientSecure;
  client->setInsecure();
  HTTPClient https;
  https.begin(*client, url);
  https.addHeader("Content-Type", "application/json");
  https.addHeader("x-api-key", EXTERNAL_API_KEY);
  int httpCode = https.POST(json);
  const char* response = https.getString().c_str();
  https.end();

  if ((httpCode == HTTP_STATUS_OK) || (httpCode == HTTP_STATUS_CREATED)) 
  {
    Serial.println("Request successful!");
    Serial.println(response);
    return httpCode;
  } 
  else 
  {
    Serial.print("Request failed with error code: ");
    Serial.println(httpCode);
    Serial.println(response);
    return httpCode;
  }
}
