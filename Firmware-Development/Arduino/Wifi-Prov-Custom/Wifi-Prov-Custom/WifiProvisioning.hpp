#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <Preferences.h>
#include <HTTPClient.h>
#include "Device.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"

#define HTTP_STATUS_OK 200
#define HTTP_STATUS_CREATED 201

// Retry parameters
#define  SECONDS 1000
#define  RETRY_INTERVAL 10 // 10 seconds
#define  MAX_RETRY_DURATION 2*60 // 2 minutes

#define WIFI_RETRY 3  // number of times to retry WiFi connection

#define EXTERNAL_API_KEY "b8z6X5fzbjNDGHz15XtPVzEkO3h5mCT7q4Ei8AupX9DD12PxL94rhnZHarHc8O2H"
typedef enum
{
  TELEMETRY_NOTSENT,
  TELEMETRY_SENT,
} telemetryStatus;

class WifiProvisioning 
{
  private:
    // Replace with your API details
    const char* apiHost = "smartmasjid-api-qa.futuriklabs.com";
    const char* apiEndpointTemplate = "/external/device/telemetry";
    
    const char* macAddress = MAC_ADDRESS;
    char gSsid[32];
    char gPass[32];
    
    Preferences preferences;
    telemetryStatus deviceTelemetryStatus = TELEMETRY_NOTSENT;
    
  public:
    bool connectWifiFromNVS(void);
    bool connectWifi(const char* ssid, const char* pass);
    void wifiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
    bool sendDeviceTelemetry(const char* macAddress, uint8_t retryInterval, uint16_t maxRetryDuration);
    bool sendDeviceTelemetry(const char* macAddress);
    bool storeWifiCredsNVS(void);
    bool storeWifiCredsNVS(const char* ssid, const char* pass);
    bool storeWifiCredsRAM(const char* ssid, const char* pass);
    bool isTelemetrySent(void);
};
