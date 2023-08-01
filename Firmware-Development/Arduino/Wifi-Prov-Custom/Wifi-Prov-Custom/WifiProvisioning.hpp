#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <Preferences.h>
#include <HTTPClient.h>
#include "Device.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"

// Retry parameters
#define  RETRY_INTERVAL 10 // 10 seconds
#define  MAX_RETRY_DURATION 2*60 // 2 minutes

#define WIFI_RETRY 3  // number of times to retry WiFi connection

typedef enum
{
  TELEMETRY_NOTSENT,
  TELEMETRY_SENT,
} telemetryStatus;

class WifiProvisioning 
{
  private:
    char gSsid[32];
    char gPass[32];
    
    Preferences preferences; 
    telemetryStatus deviceTelemetryStatus = TELEMETRY_NOTSENT;
    
  public:
    bool connectWifiFromNVS(void);
    bool connectWifi(const char* ssid, const char* pass);
    void wifiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
    bool storeWifiCredsNVS(void);
    bool storeWifiCredsNVS(const char* ssid, const char* pass);
    bool storeWifiCredsRAM(const char* ssid, const char* pass);
    bool isTelemetrySent(void);
};
