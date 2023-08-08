#include "BLE.hpp"
#include "Button.hpp"
#include "SM_Http.hpp"
#include "WifiProvisioning.hpp"
#include "Mqtt.hpp"
#include "Device.hpp"

WifiProvisioning wifiProv;
BLE ble;
Button button(SMBTN_GPIO);
SmHttp smHttp;
MQTTClient mqtt(&wifiProv);

void setup() {
  Serial.begin(115200);

  button.setup();

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
  mqtt.setup();
  while (1)
  {
    button.loop();
    mqtt.loop();
  }
}
