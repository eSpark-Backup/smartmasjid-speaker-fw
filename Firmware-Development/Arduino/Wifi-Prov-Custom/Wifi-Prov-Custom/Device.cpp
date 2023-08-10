#include <WiFi.h>
#include "SM_Nvs.hpp"
#include "Device.hpp"

char Device::macAddress[20] = "";

bool Device::getDeviceMAC(char* dest) 
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

void Device::setup() 
{
  getDeviceMAC(macAddress);

  // For Testing
  Serial.println(Device::macAddress); 
  char masjidId[32];
  bool ret = Sm_Nvs::get(DEVICE_FACTORY_INFO_NAMESPACE,
                        std::make_pair("masjidId", masjidId)); 
  if(ret)
  {
    Serial.print("masjidId: ");
    Serial.println(masjidId);   
  }
  else
  {
    Serial.println("Unable to find masjid id");
  }
}
