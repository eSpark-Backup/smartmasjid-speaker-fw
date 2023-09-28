#pragma once

#include <WiFiClientSecure.h>
#include "Mqtt.hpp"
#define HTTP_STATUS_OK 200
#define HTTP_STATUS_CREATED 201
#define EXTERNAL_API_KEY "b8z6X5fzbjNDGHz15XtPVzEkO3h5mCT7q4Ei8AupX9DD12PxL94rhnZHarHc8O2H"
#define TO_MS(seconds) (seconds * 1000)
typedef int httpcode;

#define HTTP_OK 200
#define HTTP_404_ERROR 404

typedef enum
{
 POST = 0,
 PATCH,
}http;

class SmHttp 
{
  private:
    const char* smHost = "https://smartmasjid-api-qa.futuriklabs.com";
    const char* telemetryEndpoint = "/external/device/telemetry";
    const char* adhanStartEndpoint = "/external/adhan/start";
    const char* adhanStopEndpoint = "/external/adhan/stop";
    const char* adhanRestEndpoint = "/external/adhan/reset";
    char MASJID_TEST_ID = '3';

  public:
    
    httpcode startAdhan();  
    httpcode sendDeviceTelemetry(const char* macAddress, uint8_t retryInterval, uint16_t maxRetryDuration);
    httpcode sendDeviceTelemetry(const char* macAddress);
    httpcode stopAdhan(); 
    httpcode resetAdhan();
    httpcode request(const char* url, const char* json, http request = POST);
};
