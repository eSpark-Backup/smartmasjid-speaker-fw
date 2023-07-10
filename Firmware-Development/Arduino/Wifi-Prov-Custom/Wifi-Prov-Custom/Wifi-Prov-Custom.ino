#include "BLE.hpp"
#include "WifiProvisioning.hpp"
#include "Mqtt.hpp"

WifiProvisioning wifiProv;
MQTTClient mqtt(&wifiProv);
BLE ble;

void setup() {
  Serial.begin(115200);
  
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
  mqtt.connectToBroker();
  mqtt.subscribeTopics();
  while (1)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      wifiProv.connectWifiFromNVS();
    }
    mqtt.loop();
  }
}
