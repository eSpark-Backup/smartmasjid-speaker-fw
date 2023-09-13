#include "BLE.hpp"
#include "Button.hpp"
#include "SM_Http.hpp"
#include "WifiProvisioning.hpp"
#include "Mqtt.hpp"
#include "Device.hpp"
#include "SM_Nvs.hpp"

WifiProvisioning wifiProv;
Button button(SMBTN_GPIO);
SmHttp smHttp;
MQTTClient mqtt(&wifiProv);

void setup() 
{
  Serial.begin(115200);
  Device::checkMacAddress();
  BLE::ble(&wifiProv);
  if(wifiProv.connectWifiFromNVS())
  {
    while(!wifiProv.isTelemetrySent())
    {
      delay(100);
    }
  } 
  else
  {
    // Turn off wifi
    WiFi.mode(WIFI_OFF);
    delay(100);
    
    // switch to Bluetooth mode
    Serial.println("Switching to Bluetooth mode");
    BLE::initialize();
    while(WiFi.status() != WL_CONNECTED);
    while (1)
    {
      if (BLE::isStarted() && wifiProv.isTelemetrySent())
      {
          BLE::stop();
          break;
      }  
      delay(100);
    }
  }
  mqtt.setup();
  // button.setup();
}

void loop() 
{ 
  button.loop();
  mqtt.loop();
}
