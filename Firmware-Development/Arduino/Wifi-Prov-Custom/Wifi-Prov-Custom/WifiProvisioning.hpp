#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <Preferences.h>
#include <HTTPClient.h>

#define HTTP_STATUS_OK 200
#define HTTP_STATUS_CREATED 201

#define WIFI_RETRY 3  // number of times to retry WiFi connection

extern Preferences preferences;

bool connectWifiFromNVS(void);
bool connectWifi(const char* ssid, const char* pass);
void wifiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
bool sendDeviceTelemetry(const char* mac_address);
void createTelemetryJson(char* json, uint8_t size, const char* status, int battery);
