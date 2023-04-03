#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>

#define WIFI_RETRY 3  // number of times to retry WiFi connection

extern Preferences preferences;

bool connectWifiFromNVS(void);
bool connectWifi(const char* ssid, const char* pass);
