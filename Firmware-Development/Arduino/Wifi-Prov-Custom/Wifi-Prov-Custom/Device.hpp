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
#define TIME_DELAY 10
#define TELEMETRY_UPDATE_TIME 1 * 60 // telemetry update time in seconds

class Device 
{
  private:
    static hw_timer_t * timer;      //H/W timer defining (Pointer to the Structure)
    static bool telemFlag;
    
  public:
    Device() = delete;  // Disable constructor
    static char macAddress[20];
    static void setup();
    static void timerSetup();
    static bool storeSubscription(const char* topic);
    static bool deleteSubscription(const char* topic);
    static bool deleteSubscriptions(); 
    static bool deleteWificredentials();
    static bool updateWifiCredential(const char* ssid, const char* pass);
    static void getDeviceMacAddress(char* bleMacAddress);
    static bool checkMacAddress();
    static bool storeMacAddress();
    static void loop();
    static void IRAM_ATTR updateTelemetry();
};
