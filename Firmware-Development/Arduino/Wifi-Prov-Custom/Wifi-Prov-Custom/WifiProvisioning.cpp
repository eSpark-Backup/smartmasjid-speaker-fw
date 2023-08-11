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
      WiFi.disconnect();
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
bool WifiProvisioning::sendDeviceTelemetry(const char* macAddress, uint8_t retryInterval, uint16_t maxRetryDuration) 
{
  // Start retry loop
  unsigned long startTime = millis();
  while (millis() - startTime < (maxRetryDuration * SECONDS)) 
  {
    if (sendDeviceTelemetry(macAddress)) 
    {
      return true;
    }
    Serial.println("Failed to send telemetry, retrying in 10 seconds...");
    delay(retryInterval * SECONDS);
  }
  return false;
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
  preferences.begin("credentials", false);
  preferences.putString("ssid", ssid);
  preferences.putString("pass", pass);
  preferences.end();
  return true;
}
void WifiProvisioning::saveSubscriptioninNVS(const char* topics)//valid
{          
  preferences.begin("Subscription");
  uint32_t c =0;
  c = preferences.getUInt("count", 0xFFFFFFFF);
  if ( c== 0xFFFFFFFF)
  {
    c =0;
  }
  preferences.putString(String(c,DEC).c_str(),topics);
  preferences.putUInt("count",c+1);
  preferences.end(); 
}  
void WifiProvisioning::deleteSubscriptioninNVS(const char* topics) //valid
{
  preferences.begin("Subscription");
  uint32_t count = preferences.getUInt("count");
  for(int i =0 ; i<=count;i++)
  {
    String key_value= preferences.getString(String(i,DEC).c_str(),"");
    bool found = false;
    if (strcmp(key_value.c_str(),topics)==0)
    {
      preferences.remove(String(i,DEC).c_str());
      key_value = preferences.getString(String(count-1,DEC).c_str(),"");
      preferences.putString(String(i,DEC).c_str(),key_value);
      count--;
      preferences.putUInt("count",count);
      found = true;
      break;
    }
  } 
    preferences.end(); 
}
void WifiProvisioning::getSubscriptionsfromNVS(void) //valid
  { 
    preferences.begin("Subscription");
    for (int i =0 ; i<=100;i++)
    {
      String key_value = preferences.getString(String(i,DEC).c_str(),"");
      if (key_value =="")
        break;
      Serial.print("subscription =");
      Serial.print(key_value);
      Serial.println("\n");
 
    }
  } 
bool WifiProvisioning::removeWifiCredsNVS(void)
{
  Serial.println("Removing Wifi Creds");
  removeWifiCredsNVS(gSsid, gPass);
  return true; 
}
bool WifiProvisioning::removeWifiCredsNVS(const char* ssid, const char* pass)
{
  preferences.remove("ssid");
  preferences.remove("pass");
  return true;
}

bool WifiProvisioning::isTelemetrySent(void)
{
  return (deviceTelemetryStatus==TELEMETRY_SENT);
}
