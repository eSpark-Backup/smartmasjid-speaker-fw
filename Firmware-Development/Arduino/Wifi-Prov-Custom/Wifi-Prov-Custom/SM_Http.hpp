#pragma once

#define HTTP_STATUS_OK 200
#define HTTP_STATUS_CREATED 201

#define EXTERNAL_API_KEY "b8z6X5fzbjNDGHz15XtPVzEkO3h5mCT7q4Ei8AupX9DD12PxL94rhnZHarHc8O2H"
#define TO_MS(seconds) (seconds * 1000)

class SmHttp 
{
  private:
    const char* smHost = "smartmasjid-api-qa.futuriklabs.com";
    const char* telemetryEndpoint = "/external/device/telemetry";
    const char* adhanStartEndpoint = "/external/adhan/start";
    
  public:
    bool startAdhan();  
    bool sendDeviceTelemetry(const char* macAddress, uint8_t retryInterval, uint16_t maxRetryDuration);
    bool sendDeviceTelemetry(const char* macAddress);
};
