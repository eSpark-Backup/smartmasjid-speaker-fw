#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <Preferences.h>
#include <HTTPClient.h>

#define HTTP_STATUS_OK 200
#define HTTP_STATUS_CREATED 201

// Retry parameters
#define  SECONDS 1000
#define  RETRY_INTERVAL 10 // 10 seconds
#define  MAX_RETRY_DURATION 2*60 // 2 minutes

#define WIFI_RETRY 3  // number of times to retry WiFi connection

extern Preferences preferences;
extern bool deviceTelmetryStatus;

bool connectWifiFromNVS(void);
bool connectWifi(const char* ssid, const char* pass);
void wifiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
bool sendDeviceTelemetry(const char* mac_address, uint8_t retry_interval, uint16_t max_retry_duration);
bool sendDeviceTelemetry(const char* mac_address);
void createTelemetryJson(char* json, uint8_t size, const char* status, int battery);
