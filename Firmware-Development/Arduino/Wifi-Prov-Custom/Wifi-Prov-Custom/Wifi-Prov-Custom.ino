#include "BLE.hpp"
#include "SM_Http.hpp"
#include "WifiProvisioning.hpp"
#include "Mqtt.hpp"
#include "Device.hpp"

WifiProvisioning wifiProv;
SmHttp smHttp;
MQTTClient mqtt(&wifiProv);

void setup() 
{
  Serial.begin(115200);
  Device::checkMacAddress();
  BLE::setup(&wifiProv);
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
      { 
        BLE::stop();
        delay(100);
      }
  }
  
  mqtt.setup();
}
 
void loop() 
{ 
  mqtt.loop();
}
