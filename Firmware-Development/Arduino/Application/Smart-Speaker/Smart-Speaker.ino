
#include "BLE.hpp"
#include "WifiProvisioning.hpp"
#include "Mqtt.hpp"
#include "Device.hpp"
#include "Button.hpp"
#include "Battery.hpp"

SET_LOOP_TASK_STACK_SIZE( 16*1024 ); // 16KB

WifiProvisioning wifiProv;
MQTTClient mqtt(&wifiProv);
Button button;
Battery battery;


void setup() 
{
  Serial.begin(115200);
  LED& rgb = *LED::getInstance();
  rgb.createTask();
  battery.setUp();
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
  button.setUp(&mqtt);
}
 
void loop() 
{
  Device::loop();  
  button.loop(); 
  battery.loop();
  Device::memoryMonitoring();
}
