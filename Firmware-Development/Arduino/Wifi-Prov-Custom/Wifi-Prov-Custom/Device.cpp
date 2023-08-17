#include <WiFi.h>
#include "SM_Nvs.hpp"
#include "Device.hpp"

char Device::macAddress[20] = "";

bool Device::getMac(char* dest) {
  if (!dest) {
    Serial.println("getDeviceMAC() -> dest* is null");
    return false;
  }

  uint8_t mac[6];
  WiFi.macAddress(mac);
  mac[5] += BLE_MAC_OFFSET;

  sprintf(dest, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return true;
}

void Device::setup() {
  getMac(macAddress);

  // For Testing
  Serial.println(Device::macAddress);
  char masjidId[32];
  bool ret = Sm_Nvs::get(DEVICE_FACTORY_INFO_NAMESPACE,
                         std::make_pair("masjidId", masjidId));
  if (ret) {
    Serial.print("masjidId: ");
    Serial.println(masjidId);
  } else {
    Serial.println("Unable to find masjid id");
  }
}
bool Device::storeSubscribeTopicstoNVS(const char* top) {
  uint32_t c = 0;
  if (!Sm_Nvs::get(DEVICE_SUBSCRIPTION_NAMESPACE,
                   std::make_pair("count", &c)))
    return false;


  if (!Sm_Nvs::put(DEVICE_SUBSCRIPTION_NAMESPACE,
                   std::make_pair(String(c, DEC).c_str(), top)))
    return false;

  c++;
  if (!Sm_Nvs::put(DEVICE_SUBSCRIPTION_NAMESPACE,
                   std::make_pair("count", c)))
    return false;

  return true;
}
bool Device::deleteSubscribeTopicstoNVS(const char* topic) 
{
  uint32_t count;
  Sm_Nvs::get(DEVICE_SUBSCRIPTION_NAMESPACE, std::make_pair("count", &count));
  char keyValue[50];
  bool topicFound = false;
  for (int i = 0; i < count; i++) 
  {
    Sm_Nvs::get(DEVICE_SUBSCRIPTION_NAMESPACE, std::make_pair(String(i, DEC).c_str(), keyValue));
    if (strcmp(keyValue, topic) == 0)
     {
      topicFound = true;
      if(!Sm_Nvs::del(DEVICE_SUBSCRIPTION_NAMESPACE, String(i, DEC).c_str()))
         return false;
      Sm_Nvs::get(DEVICE_SUBSCRIPTION_NAMESPACE, std::make_pair(String(count - 1, DEC).c_str(), keyValue));
      Sm_Nvs::put(DEVICE_SUBSCRIPTION_NAMESPACE, std::make_pair(String(i, DEC).c_str(), keyValue));
      count--;
      Sm_Nvs::put(DEVICE_SUBSCRIPTION_NAMESPACE, std::make_pair("count", count));
      break;
    }
  }

  return topicFound;  
}
