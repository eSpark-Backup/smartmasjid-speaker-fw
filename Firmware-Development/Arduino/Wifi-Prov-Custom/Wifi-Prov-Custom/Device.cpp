#include <WiFi.h>
#include "SM_Json.hpp"
#include "SM_Nvs.hpp"
#include "Device.hpp"

char Device::macAddress[20] = "";

bool Device::getMac(char* dest) 
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
  getMac(macAddress);

  // For Testing
  Serial.println(Device::macAddress);
  char masjidId[32];
  bool ret = Sm_Nvs::get(DEVICE_FACTORY_INFO_NAMESPACE,
                         std::make_pair("masjidId", masjidId));
  if (ret) 
  {
    Serial.print("masjidId: ");
    Serial.println(masjidId);
  } 
  else 
  {
    Serial.println("Unable to find masjid id");
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
  if (!Sm_Nvs::put(DEVICE_WIFI_CREDENTIALS_NAMESPACE,std::make_pair("ssid", ssid), std::make_pair("pass", pass))) 
    return false;
  Serial.println("Wifi credentials updated");
  return true;
}
