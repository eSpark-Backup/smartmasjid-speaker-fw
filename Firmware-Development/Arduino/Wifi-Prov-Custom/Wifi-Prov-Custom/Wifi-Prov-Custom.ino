#include "BLE.hpp"
#include "WifiProvisioning.hpp"

void setup() {
  Serial.begin(115200);
  
  if(connectWifiFromNVS())
  {
    return;
  }
  
  // switch to Bluetooth mode
  Serial.println("Switching to Bluetooth mode");
  initializeBLE();
  while(WiFi.status() != WL_CONNECTED);
}

void loop() {
   if (bleStarted && deviceTelmetryStatus)
   {
      btStop();
      bleStarted = false;
      Serial.println("BLE Stopped");  
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    connectWifiFromNVS();
  }
}
