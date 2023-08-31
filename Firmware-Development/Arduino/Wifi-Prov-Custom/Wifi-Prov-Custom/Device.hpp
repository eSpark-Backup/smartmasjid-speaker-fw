#pragma once
#include <WiFi.h>

#define SM_MODEL "SSB23V1"
#define SM_FIRMWARE_VER "1.0"
#define BLE_MAC_OFFSET 2
#define BATTERY_HEALTH 90
#define MASJID_ID_TEST 3
#define DEVICE_FACTORY_INFO_NAMESPACE "factoryInfo"
#define DEVICE_WIFI_CREDENTIALS_NAMESPACE "credentials"
#define DEVICE_SUBSCRIPTION_NAMESPACE "subscription"
#define timeDelay 10

class Device 
{
  private:
    static bool getMac(char* dest);

  public:
    Device() = delete;  // Disable constructor
    static char macAddress[20];

    static void setup();
    static bool storeSubscription(const char* topic);
    static bool deleteSubscription(const char* topic);
    static bool deleteSubscriptions(); 
    static bool deleteMasjidId();
    static bool deleteWificredentials();
    static bool updateWifiCredential(const char* ssid, const char* pass);
};
