//  set esp32_rev_min 3
//  set bluetooh enable y
//  set mbed_dynamic_buffer ---enable
//  
#include "Source/BLE.hpp"
#include "Source/WifiProvisioning.hpp"
#include "Source/Mqtt.hpp"
#include "Source/Device.hpp"
#include "Source/Button.hpp"
#include "Source/Battery.hpp"
#include"Source/Amplifier.hpp"

SET_LOOP_TASK_STACK_SIZE( 16*1024 ); // 16KB

WifiProvisioning wifiProv;
MQTTClient mqtt(&wifiProv);
// Button button;
Battery battery;
Amplifier& amp = *Amplifier::getInstance();

void setup() 
{
  Serial.begin(115200);
  LED& rgb = *LED::getInstance();
  rgb.createTask();
  battery.setup();
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
   amp.setup();
  Button::setup(&mqtt);
   
}
 
void loop() 
{ 
  Device::loop();  
  battery.loop();
  Device::memoryMonitoring();
  amp.volumeLoop();
}
