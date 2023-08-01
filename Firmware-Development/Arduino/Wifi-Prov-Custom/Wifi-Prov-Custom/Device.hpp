#pragma once
#include <WiFi.h>

#define SM_MODEL "SBB23V1"
#define SM_FIRMWARE_VER "1.0"
#define BLE_MAC_OFFSET 2
#define BATTERY_HEALTH 90
#define MASJID_ID_TEST 3

static inline bool getDeviceMAC(char* dest)
{
  if (!dest)
  {
    Serial.println("getDeviceMAC() -> dest* is null");
    return false;
  }
  
  uint8_t mac[6];
  WiFi.macAddress(mac);
  mac[5] += BLE_MAC_OFFSET;

  sprintf(dest, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return true;
}
  
