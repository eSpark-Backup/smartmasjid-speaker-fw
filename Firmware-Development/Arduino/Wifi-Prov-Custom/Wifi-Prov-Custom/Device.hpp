#pragma once

#define SM_MODEL "SBB23V1"
#define SM_FIRMWARE_VER "1.0"
#define BLE_MAC_OFFSET 2
#define BATTERY_HEALTH 90
#define MASJID_ID_TEST 3
#define DEVICE_FACTORY_INFO_NAMESPACE "factoryInfo"
#define DEVICE_WIFI_CREDENTIALS_NAMESPACE "credentials"
#define timeDelay 10

class Device 
{
  private:
    static bool getMac(char* dest);
  
  public:
    Device() = delete; // Disable constructor
    static char macAddress[20];
    
    static void setup();

};
