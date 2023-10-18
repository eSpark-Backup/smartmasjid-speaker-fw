#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "Device.hpp"
#include "esp_bt_main.h"
#include "esp_bt_device.h"

// Retry parameters
#define SECONDS 1000
#define RETRY_INTERVAL 10 // 10 seconds
#define DEVICE_TELEMETRY_MAX_RETRIES  6 // 6 retries
#define WIFI_RETRY 10  // number of times to retry WiFi connection
#define TO_MS(seconds) ((seconds) * 1000)

typedef enum
{
  TELEMETRY_NOTSENT,
  TELEMETRY_SENT,
} telemetryStatus;

class IWifiProvisioning 
{
  public:
    virtual bool connectWifi(const char* ssid, const char* pass, int retries = WIFI_RETRY) = 0;
};

class WifiProvisioning : public IWifiProvisioning
{
  private:
    // Replace with your API details
    const char* apiHost = "smartmasjid-api-qa.futuriklabs.com";
    const char* apiEndpointTemplate = "/external/device/telemetry";
  
    char gSsid[32];
    char gPass[32];
    telemetryStatus deviceTelemetryStatus = TELEMETRY_NOTSENT;
    
  public:

    bool connectWifiFromNVS(void);
    bool connectWifi(const char* ssid, const char* pass, int retries = WIFI_RETRY) override;
    void wifiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
    bool sendDeviceTelemetry(const char* macAddress, uint8_t retryInterval, uint16_t maxRetryDuration);
    bool sendDeviceTelemetry(const char* macAddress);
    void createTelemetryJson(char* json, uint8_t size, const char* status, int battery);
    bool isTelemetrySent(void);
};
