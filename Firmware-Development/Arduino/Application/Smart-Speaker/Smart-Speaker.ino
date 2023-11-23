#include "BLE.hpp"
#include "SM_Http.hpp"
#include "WifiProvisioning.hpp"
#include "Mqtt.hpp"
#include "Device.hpp"

SET_LOOP_TASK_STACK_SIZE( 16*1024 ); // 16KB

WifiProvisioning wifiProv;
SmHttp smHttp;
MQTTClient mqtt(&wifiProv);

void setup() 
{
  Serial.begin(115200);
  Device::checkMacAddress();
  Serial.printf("Arduino Stack was set to %d bytes\n", getArduinoLoopTaskStackSize());
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
    while(WiFi.status() != WL_CONNECTED) 
    {
      delay(100);
    }
    BLE::stop();   
  }
  while(!wifiProv.isTelemetrySent())
  {
    delay(100);
  }
  mqtt.createTask();
  Device::timerSetup();
}
 
void loop() 
{
  Device::loop();   
  Device::memoryMonitoring();
}
