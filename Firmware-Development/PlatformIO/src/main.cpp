//  set esp32_rev_min 3
//  set bluetooh enable y
//  set mbed_dynamic_buffer ---enable
//  

#include "Source/BLE.hpp"
#include "Source/WifiProvisioning.hpp"
#include "Source/Mqtt.hpp"
#include "Source/Device.hpp"
#include "Source/Button.hpp"
// #include "Source/Battery.hpp"
#include"Source/Amplifier.hpp"
#include  "Source/LocalStorage.hpp"
#include "Source/SM_Json.hpp"
#include "Source/WifiMonitoring.hpp"

SET_LOOP_TASK_STACK_SIZE( 16*1024 ); // 16KB

WifiProvisioning wifiProv;
MQTTClient mqtt(&wifiProv);
// Button button;
// Battery battery;
Amplifier& amp = *Amplifier::getInstance();
WifiMonitoring wifiMonitor;


void setup() 
{
  Serial.begin(115200);
  localStorage.setup();
  LED& rgb = *LED::getInstance();
  rgb.createTask();
  // battery.setup();
  rgb.currentMode = MODE::ON_POWER_UP;   
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
  char device[70];
  char smFirmware[24];
  const char* json = localStorage.get(FILE_JSON);
  Serial.println(json);
  SM_Json::parse(json , std::make_pair("device_info", device));
  SM_Json::parse((const char*)(device) , std::make_pair("firmware_ver", smFirmware));
  Serial.print("FIRMWARE VERSION : ");
  Serial.println(smFirmware);
 
  localStorage.print();
  mqtt.createTask();
   Device::timerSetup();
   amp.setup();
  Button::setup(&mqtt);
   
}
 
void loop() 
{ 
  Device::loop();  
  // battery.loop();
  Device::memoryMonitoring();
  wifiMonitor.MonitorWifi();
  amp.volumeLoop();
}
