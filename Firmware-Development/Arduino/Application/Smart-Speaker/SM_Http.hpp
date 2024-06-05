#pragma once

#include <WiFiClientSecure.h>
#include "Mqtt.hpp"
#define HTTP_STATUS_OK 200
#define HTTP_STATUS_CREATED 201
#define EXTERNAL_API_KEY "b8z6X5fzbjNDGHz15XtPVzEkO3h5mCT7q4Ei8AupX9DD12PxL94rhnZHarHc8O2H"
#define TO_MS(seconds) (seconds * 1000)

typedef int HttpStatusCode;

namespace Http
{
  enum Response
  {
    OK = 200,
    CREATED = 201,
    DEVICE_NOT_FOUND = 404,
  };
  
  enum class RequestType
  {
    POST,
    PATCH,
    PUT,
  };
}

class SmHttp 
{
  private:
    const char* smHost = "https://smartmasjid-api-qa.futuriklabs.com";
    const char* telemetryEndpoint = "/external/device/telemetry";
    const char* OtaUpdateEndpoint = "/external/device/update";

  public:
    HttpStatusCode sendDeviceTelemetry(const char* macAddress, uint8_t retryInterval, uint16_t maxRetryDuration);
    HttpStatusCode sendDeviceTelemetry(const char* macAddress);
    HttpStatusCode put(const char* macAddress);
    HttpStatusCode post(const char* url, const char* json, Http::RequestType request =  Http::RequestType::POST);
    
    
};

extern SemaphoreHandle_t httpsSemaphore;