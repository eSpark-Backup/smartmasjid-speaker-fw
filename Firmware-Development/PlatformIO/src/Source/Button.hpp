#pragma once

#undef IRAM_ATTR
#define IRAM_ATTR

#include <Arduino.h>
#include "Mqtt.hpp"

// Button Configuration
#define UP_BUTTON   14
#define DOWN_BUTTON 13
//#define MUTE_BUTTON -1  //GPIO 16 can not be used because of PSRAM (GPIO16 = CS pin)
#define RESET_BUTTON 22 
#define DEBOUNCE_DELAY 200

class Button
{
  private:
    static const uint32_t PRESS_DURATION = 5000; // 5 seconds
    static bool resetButtonPressed;
    static void resetbutton();
    static void volumeUpWrapper();
    static void volumeDownWrapper(); 
    static void volumeMuteWrapper();
    static void resetButtonTask(void *parameter);
    static void IRAM_ATTR resetButtonISR();
    static void interruptHandler();
    static MQTTClient* pMqtt;

  public:
    static volatile bool volumeUpFlag;
    static volatile bool volumeDownFlag;
    static volatile bool volumeMuteFlag;

    static volatile unsigned long lastPressTimeUp;
    static volatile unsigned long lastPressTimeDown;

    static void setup(MQTTClient* mqtt);
};
