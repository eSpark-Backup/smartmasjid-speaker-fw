#include "BLE.hpp"
#include "SM_Http.hpp"
#include "WifiProvisioning.hpp"
#include "Mqtt.hpp"
#include "Device.hpp"
#include"Amplifier.hpp"

SET_LOOP_TASK_STACK_SIZE( 16*1024 ); // 16KB

WifiProvisioning wifiProv;
// // SmHttp smHttp;
MQTTClient mqtt(&wifiProv);


void setup() 
{

  Serial.begin(115200);
  
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(ssid.c_str(), password.c_str());

  // while (WiFi.status() != WL_CONNECTED)

  // {
  //   Serial.print(".");
  //   delay(1000);
  // }
  Device::checkMacAddress();
  Serial.printf("Arduino Stack was set to %d bytes\n", getArduinoLoopTaskStackSize());
  // ble.setup(&wifiProv);
  // WiFi.mode(WIFI_STA);
  if(wifiProv.connectWifiFromNVS()){}
  //   {
  //     while(!wifiProv.isTelemetrySent())
  //     {
  //       delay(100);
  //     }
  //   } 
  // else
  //   {
  //     // Turn off wifi
  //     WiFi.mode(WIFI_OFF);
  //     delay(100);
      
  //     // switch to Bluetooth mode
  //     Serial.println("Switching to Bluetooth mode");
  //     ble.initialize();
  //     while(WiFi.status() != WL_CONNECTED) 
  //     {
  //       delay(100);
  //     }
      // ble.stop();   
  //   }
  // while(!wifiProv.isTelemetrySent())
  // {
  //   delay(100);
  // }
  amp.setup();

  mqtt.createTask();
  // Device::timerSetup();
  
}
 
void loop() 
{
  // Device::loop();   
  Device::memoryMonitoring();
  // unsigned long currentTime = millis();
  // static unsigned long lastExecution = 0;
  // if (currentTime - lastExecution >= 20000)
  //  {
  //   // Amplifier::stream("STOP", "https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/Olsen-Banden.mp3");
  //   amp.stream("START", "https://www.islamcan.com/audio/adhan/azan2.mp3");
  //   lastExecution = currentTime;
  // }

  // if (i>=1)
  //   {
  //     delay(20000);
  //     amp.stream("STOP", "https://www.islamcan.com/audio/adhan/azan2.mp3");
  //   }
}


