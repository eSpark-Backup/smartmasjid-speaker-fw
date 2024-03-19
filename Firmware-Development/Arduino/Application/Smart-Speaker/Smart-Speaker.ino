
#include "BLE.hpp"
#include "SM_Http.hpp"
#include "WifiProvisioning.hpp"
#include "Mqtt.hpp"
#include "Device.hpp"
#include"Amplifier.hpp"
#include "Button.hpp"
#include "LED.hpp"

SET_LOOP_TASK_STACK_SIZE( 16*1024 ); // 16KB
// SemaphoreHandle_t ampSemaphore = NULL;  // Declare the semaphore handle globally
WifiProvisioning wifiProv;
// // SmHttp smHttp;
MQTTClient mqtt(&wifiProv);
Button button;

void setup() 
{

  Serial.begin(115200);
  rgb.createTask();
  // ampSemaphore = xSemaphoreCreateMutex();  // Initialize the semaphore
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
  amp.setup();
  mqtt.createTask();
  Device::timerSetup();
  button.setUp(&mqtt);
  
}
 
void loop() 
{
  Device::loop();  
  button.loop(); 
  Device::memoryMonitoring();
}



