#include <WiFi.h>
#include "SM_Json.hpp"
#include "SM_Nvs.hpp"
#include "Device.hpp"
#include "esp_bt_main.h"
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

bool Device::storeSubscription(const char* topic) 
{
  uint32_t c = 0;
  if (!Sm_Nvs::get(DEVICE_SUBSCRIPTION_NAMESPACE,
                   std::make_pair("count", &c)))
    return false;


  if (!Sm_Nvs::put(DEVICE_SUBSCRIPTION_NAMESPACE,
                   std::make_pair(String(c, DEC).c_str(), topic)))
    return false;

  if (!Sm_Nvs::put(DEVICE_SUBSCRIPTION_NAMESPACE,
                   std::make_pair("count", c + 1)))
    return false;

  return true;
}

bool Device::deleteSubscription(const char* topic) 
{
  uint32_t count;
  Sm_Nvs::get(DEVICE_SUBSCRIPTION_NAMESPACE, std::make_pair("count", &count));
  char keyValue[50];
  bool topicFound = false;
  for (int i = 0; i <= (count+1); i++) 
  {
    Sm_Nvs::get(DEVICE_SUBSCRIPTION_NAMESPACE, std::make_pair(String(i, DEC).c_str(), keyValue));
    if (strcmp(keyValue, topic) == 0) 
    {
      topicFound = true;
      if (!Sm_Nvs::del(DEVICE_SUBSCRIPTION_NAMESPACE, String(i, DEC).c_str()))
        return false;
      Sm_Nvs::get(DEVICE_SUBSCRIPTION_NAMESPACE, std::make_pair(String(count - 1, DEC).c_str(), keyValue));
      Sm_Nvs::put(DEVICE_SUBSCRIPTION_NAMESPACE, std::make_pair(String(i, DEC).c_str(), keyValue));
      Sm_Nvs::put(DEVICE_SUBSCRIPTION_NAMESPACE, std::make_pair("count", count - 1));
      break;
    }
  }
  return topicFound;
}

bool Device::deleteSubscriptions() 
{
  uint32_t count;
  Sm_Nvs::get(DEVICE_SUBSCRIPTION_NAMESPACE, std::make_pair("count", &count));

  for (int i = 0; i < count; i++) 
  {
      if(!Sm_Nvs::del(DEVICE_SUBSCRIPTION_NAMESPACE, String(i, DEC).c_str()))
         return false;
  }
  Serial.println("Subscription deleted from Nvs");
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
  if (!Sm_Nvs::put(DEVICE_WIFI_CREDENTIALS_NAMESPACE,std::make_pair("ssid", ssid), std::make_pair("pass", pass))) 
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