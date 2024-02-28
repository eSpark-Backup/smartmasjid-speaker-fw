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
#define DEVICE_MEMORY_INFO_PRINT_INTERVAL 5000

// Timer Settings
#define TIMER_NO 0
#define TIMER_PRESCALAR_1MHZ 80
#define TIMER_ONE_SECOND_COUNTS 1000000
#define TELEMETRY_UPDATE_TIME 1 * 60 // telemetry update time in seconds

// Button Configuration
#define UP_BUTTON   14
#define DOWN_BUTTON 16
#define MUTE_BUTTON 13
// #define RESET_BUTTON 7

class Device 
{
  private:
    // Variables
    static hw_timer_t * timer;      //H/W timer defining (Pointer to the Structure)
    static bool telemetryTrigger;   // static bool telemetryTrigger;
    
  public:
    // Constructor
    Device() = delete;  // Disable constructor
    // int UP_BUTTON = 14;
    // int DOWN_BUTTON = 16;
    // int MUTE_BUTTON = 22;
    // Variables
    static char macAddress[20];
    // Static member methods
    static void setup();
    static void loop();
    static void timerSetup();
    static bool storeSubscription(const char* topic);
    static bool deleteSubscription(const char* topic);
    static bool deleteSubscriptions(); 
    static bool deleteWificredentials();
    static bool updateWifiCredential(const char* ssid, const char* pass);
    static void getDeviceMacAddress(char* bleMacAddress);
    static bool checkMacAddress();
    static bool storeMacAddress();
    static void memoryMonitoring();
    static void buttonSetup();
    static void upbutton();
    static void downbutton();
    static void mutebutton();
    //  void getMqttInstance(MQTTClient* mqtt);
    static void resetbutton();
    static void volumeUpWrapper();
    static void volumeDownWrapper(); 
    static void volumeMuteWrapper();
    // static void resetWrapper();
    // ISR
    static void IRAM_ATTR triggerTelemetryUpdate();
};
