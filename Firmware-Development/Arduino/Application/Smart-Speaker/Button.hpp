#pragma once

#undef IRAM_ATTR
#define IRAM_ATTR

#include<Arduino.h>
// Button Configuration
#define UP_BUTTON   14
#define DOWN_BUTTON 16
#define MUTE_BUTTON 13
// #define RESET_BUTTON 7

// Button LED Configuration
#define UP_BUTTON_LED_1   19
#define DOWN_BUTTON_LED_2 18
#define MUTE_BUTTON_LED_3 4
#define RESET_BUTTON_LED_4 7

class Button
{
  private:
    static const uint8_t RESET_BUTTON = 7;
    static const uint32_t LONG_PRESS_DURATION = 5000; // 5 seconds
    static bool resetButtonPressed;
    static volatile  byte stateUpButtonLed;
    static volatile  byte stateDownButtonLed;
    static volatile  byte stateMuteButtonLed;
    static volatile  byte stateResetButtonLed;
    
    static void resetbutton();
    static void volumeUpWrapper();
    static void volumeDownWrapper(); 
    static void volumeMuteWrapper();
    
  public:
    void setUp();
    void loop();
    static void resetButtonTask(void *parameter);
    static void IRAM_ATTR resetButtonISR();
    static void resetButtonPressedHandler();
    
};