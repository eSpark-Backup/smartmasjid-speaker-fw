#include "BLE.hpp"
#include "WifiProvisioning.hpp"
#include "Mqtt.hpp"

WifiProvisioning wifiProv;
MQTTClient mqtt;
BLE ble;

void setup() {
  Serial.begin(115200);
  
  if(wifiProv.connectWifiFromNVS())
  {
    while(!deviceTelmetryStatus)
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
    if (bleStarted && deviceTelmetryStatus)
    {
        wifiProv.storeWifiCredsNVS();
        ble.stop();
        bleStarted = false;
        Serial.println("BLE Stopped"); 
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
