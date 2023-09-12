#include <WiFi.h>
#include "SM_Nvs.hpp"
#include "Device.hpp"
#include "esp_bt_device.h"

char Device::macAddress[20] = "";

void Device::setup() 
{

  if(checkMacAddress())
  {
   Serial.println(Device::macAddress);
  }
  else
  {
    Device::getDeviceMacAddress(Device::macAddress);
    storeMacAddress();
  }
 
}

bool Device::deleteMasjidId() 
{
  if (!Sm_Nvs::del(DEVICE_FACTORY_INFO_NAMESPACE, "masjidId")) 
    return false;
  Serial.println("Masjid Id Removed");
  return true;
}
bool Device::deleteWificredentials() 
{
  if (!Sm_Nvs::del(DEVICE_WIFI_CREDENTIALS_NAMESPACE, "ssid", "pass")) 
    return false;
  Serial.println("Old credential removed");
  return true;
}

bool Device::updateWifiCredential(const char* ssid, const char* pass) 
{
  if (!Sm_Nvs::put(DEVICE_WIFI_CREDENTIALS_NAMESPACE,
                  std::make_pair("ssid", ssid),
                  std::make_pair("pass", pass))) 
    return false;
  Serial.println("Wifi credentials updated");
  return true;
}

bool Device::checkMacAddress()
{
  if (Sm_Nvs::get(DEVICE_FACTORY_INFO_NAMESPACE, std::make_pair("macaddress", Device::macAddress)))
  {
    return true;
  }
  return false;
}

bool Device::storeMacAddress()
{
  
  if (Sm_Nvs::put(DEVICE_FACTORY_INFO_NAMESPACE, std::make_pair("macaddress", Device::macAddress)))
  {
    Serial.print("Updated MAC address in NVS: ");
    Serial.println(Device::macAddress);
    return true;
  }
  else
  {
    Serial.println("Failed to update MAC address in NVS.");
    return false;
  }
}

void Device::getDeviceMacAddress(char* bleMacAddress) {
  const uint8_t* point = esp_bt_dev_get_address();

  for (int i = 0; i < 6; i++) 
  {
    sprintf(bleMacAddress, "%02X", (int)point[i]);
    bleMacAddress += 2;
    if (i < 5) 
    {
      sprintf(bleMacAddress, ":");
      bleMacAddress += 1;
    }
  }
}
