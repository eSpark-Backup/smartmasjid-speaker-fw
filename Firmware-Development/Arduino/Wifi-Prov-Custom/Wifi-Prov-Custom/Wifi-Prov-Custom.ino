#include "BLE.hpp"
#include "WifiProvisioning.hpp"
#include "Mqtt.hpp"
MQTTClient mqtt;

void setup() {
  Serial.begin(115200);
  
  if(connectWifiFromNVS())
  {
    while(!deviceTelmetryStatus)
    {
      delay(100);
    }
    return;
  }
  
  // switch to Bluetooth mode
  Serial.println("Switching to Bluetooth mode");
  initializeBLE();
  while(WiFi.status() != WL_CONNECTED);
  while (1)
  {
    if (bleStarted && deviceTelmetryStatus)
    {
        storeWifiCredsNVS();
        btStop();
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
      connectWifiFromNVS();
    }
    mqtt.loop();
  }
}
