
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "SM_Http.hpp"
#include "SM_Json.hpp"
#include "Device.hpp"
#include"LocalStorage.hpp"


HttpStatusCode SmHttp::sendDeviceTelemetry(const char* macAddress, uint8_t retryInterval, uint16_t maxRetry)
{  
  HttpStatusCode status = Http::Response::DEVICE_NOT_FOUND;
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
  char http[150];
  char smHost[50];
 
  const char* json = localStorage.get(FILE_JSON);
  SM_Json::parse(json , std::make_pair("http", http));
  SM_Json::parse((const char*)(http) , std::make_pair("host", smHost));
  
  std::string deviceTelemetryUrl = smHost;
  deviceTelemetryUrl += telemetryEndpoint;

  char telemetryJson[64];
  SM_Json::create(telemetryJson, sizeof(telemetryJson),
                  std::make_pair("battery", BATTERY_HEALTH),
                  std::make_pair("macAddress", Device::macAddress));
  Serial.println(telemetryJson);
  return request(deviceTelemetryUrl.c_str(),telemetryJson);
}

HttpStatusCode SmHttp::notifyOTAUpdate(const char* macAddress) 
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Wifi not connected : Unable to send request");
    return false;
  }
  char http[150];
  char smHost[50];
  char device[70];
  char smFirmware[6];

  const char* json = localStorage.get(FILE_JSON);
  
  SM_Json::parse(json , std::make_pair("http", http));
  SM_Json::parse((const char*)(http) , std::make_pair("host", smHost));
  
  std::string updateOnLoadUrl = smHost;
  updateOnLoadUrl += otaUpdateEndpoint;
  char payload[64];

  SM_Json::parse(json , std::make_pair("device_info", device));
  SM_Json::parse((const char*)(device) , std::make_pair("firmware_ver", smFirmware));
  
  SM_Json::create(payload, sizeof(payload),
                  std::make_pair("macAddress", Device::macAddress),
                  std::make_pair("firmwareVersion", smFirmware));
  Serial.println(payload);
  return request(updateOnLoadUrl.c_str(), payload, Http::RequestType::PUT);
  
}

HttpStatusCode SmHttp::updateDeviceStatus(const char* macAddress, char* status)
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Wifi not connected : Unable to send request");
    return false;
  }
  char http[150];
  char smHost[50];
 
  const char* json = localStorage.get(FILE_JSON);
  SM_Json::parse(json , std::make_pair("http", http));
  SM_Json::parse((const char*)(http) , std::make_pair("host", smHost));
  std::string updateOnLoadUrl = smHost;
  updateOnLoadUrl += otaEndpoint;

  char payload[64];
  SM_Json::create(payload, sizeof(payload),
                  std::make_pair("macAddress", Device::macAddress),
                  std::make_pair("status", status));
  Serial.println(payload);
  return request(updateOnLoadUrl.c_str(), payload, Http::RequestType::PUT);
}

HttpStatusCode SmHttp::request(const char* url, const char* json, Http::RequestType request) 
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Wifi not connected : Unable to send request");
    return false;
  }
  char http[150];
  char externalApiKey[70];
  const char* jsonFile = localStorage.get(FILE_JSON);
  SM_Json::parse(jsonFile , std::make_pair("http", http));
  SM_Json::parse((const char*)(http) , std::make_pair("api_key", externalApiKey));
  
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient https;
  https.setReuse(false);
  https.begin(client, url);
  https.addHeader("Content-Type", "application/json");
  https.addHeader("x-api-key", externalApiKey);

  HttpStatusCode status = Http::Response::DEVICE_NOT_FOUND;

  switch(request)
  {
    case Http::RequestType::POST:
       status = https.POST(json);
       break;
    case Http::RequestType::PATCH:
       status = https.PATCH(json);
       break;
    case Http::RequestType::PUT: 
        status = https.PUT(json);
        break;  
  
  }

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
    // Serial.println(status);
  }

  Serial.println(response);
  return status;
}

