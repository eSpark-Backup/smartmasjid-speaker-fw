#include "BLE.hpp"
#include "button.hpp"
#include "HTTP_SM.hpp"
#include "WifiProvisioning.hpp"
#include "Device.h"

WifiProvisioning wifiProv;
BLE ble;
Button btt;
SmHttp smHttp;

void setup() {
  Serial.begin(115200);

  btt.setup();

  if(wifiProv.connectWifiFromNVS())
  {
    while(!wifiProv.isTelemetrySent())
    {
      delay(100);
    }
    return;
  } 
  // switch to Bluetooth mode
  Serial.println("Switching to Bluetooth mode");
  ble.initialize(&wifiProv);
  while(WiFi.status() != WL_CONNECTED);
  while (1)
  {
    if (ble.isStarted() && wifiProv.isTelemetrySent())
    {
        wifiProv.storeWifiCredsNVS();
        ble.stop();
        return; 
    }  
    delay(100);
  }
  
}

void loop() 
{ 
   if(btt.isPressed())
    {
      Serial.println("Button Pressed");
      smHttp.startAdhan();
      btt.clearState();
    }

}
