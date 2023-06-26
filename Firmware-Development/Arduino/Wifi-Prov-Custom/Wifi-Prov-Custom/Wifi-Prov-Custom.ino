#include "BLE.hpp"
#include "WifiProvisioning.hpp"
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP32WiFi.h>
#endif
WifiProvisioning wifiProv;
BLE ble;

void setup() {
  Serial.begin(115200);
  Serial.print("ESP Board MAC Address: \n ");
  Serial.println(WiFi.macAddress());
  
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

void loop() {

}
