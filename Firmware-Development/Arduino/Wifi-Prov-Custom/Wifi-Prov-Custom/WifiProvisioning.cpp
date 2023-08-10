#include "WifiProvisioning.hpp"
#include "Device.hpp"
#include "BLE.hpp"
#include "SM_Http.hpp"
#include "SM_Nvs.hpp"

bool WifiProvisioning::connectWifi(const char *ssid, const char *pass, int retries)
{ 
  if (ssid != NULL && pass != NULL)
  {
    Serial.printf("Connecting to %s...\n", ssid);
    WiFi.onEvent(std::bind(&WifiProvisioning::wifiGotIP, this, std::placeholders::_1, std::placeholders::_2), WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED && retries-- > 0)
    {
      delay(1000);
      Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    else
    {
      Serial.println("Failed to connect to WiFi");
    }
  }
  return false;
}

bool WifiProvisioning::connectWifiFromNVS(void)
{
  char ssid[32];
  char pass[32];
  
  bool ret = Sm_Nvs::get(DEVICE_WIFI_CREDENTIALS_NAMESPACE,
                        std::make_pair("ssid", ssid),
                        std::make_pair("pass", pass));   
                                              
  if (!ret)
  {
    Serial.println("No values saved for ssid or password");
  }
  else if (connectWifi(ssid, pass))
  {
    return true;
  }
  
  return false;
}

void WifiProvisioning::wifiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) 
{
  // Print network details
  Serial.print("Connected to Wi-Fi network with IP address: ");
  Serial.println(WiFi.localIP());

  char deviceMacAddress[20];
  getDeviceMAC(deviceMacAddress);

  SmHttp smHttp;
  if (smHttp.sendDeviceTelemetry(deviceMacAddress, RETRY_INTERVAL, MAX_RETRY_DURATION))
  {
    deviceTelemetryStatus = TELEMETRY_SENT;
  }
}

bool WifiProvisioning::storeWifiCredsRAM(const char* ssid, const char* pass)
{
  strcpy(gSsid, ssid);
  strcpy(gPass, pass);
  return true;
}

bool WifiProvisioning::storeWifiCredsNVS(void)
{
  storeWifiCredsNVS(gSsid, gPass);
  return true; 
}

bool WifiProvisioning::storeWifiCredsNVS(const char* ssid, const char* pass)
{
  preferences.putString("ssid", ssid);
  preferences.putString("pass", pass);
  return true;
}

bool WifiProvisioning::isTelemetrySent(void)
{
  return (deviceTelemetryStatus==TELEMETRY_SENT);
}
