#pragma once

#undef IRAM_ATTR
#define IRAM_ATTR

#include <Arduino.h>
#include "Mqtt.hpp"

// Button Configuration
#define UP_BUTTON   14
#define DOWN_BUTTON 13
//#define MUTE_BUTTON 16
#define RESET_BUTTON -1 //button is not decideed in the board


class Button
{
  private:
    static const uint32_t PRESS_DURATION = 5000; // 5 seconds
    static bool resetButtonPressed;
    static volatile  byte stateUpButtonLed;
    static volatile  byte stateDownButtonLed;
    static volatile  byte stateMuteButtonLed;
    static volatile  byte stateResetButtonLed;
    
    static void resetbutton();
    static void volumeUpWrapper();
    static void volumeDownWrapper(); 
    static void volumeMuteWrapper();
    static MQTTClient* pMqtt;

  public:
    void setup(MQTTClient* mqtt);
    static void resetButtonTask(void *parameter);
    static void IRAM_ATTR resetButtonISR();

};
