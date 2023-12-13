#pragma once
#include<Arduino.h>
#include "Audio.h"

//MAX98357AETE+T Amplifier IC pin
#define I2S_DOUT     25
#define I2S_BCLK     27
#define I2S_LRC      26
void amplifier(void* pvParameters);

class Amplifier
{
  private:
    static Audio audio;
    // Set Speaker volume
    static uint8_t volume;  //default 0.....21
    
    static void start(const char* url = NULL);
    static void stop();
    
  public:  
    static char currentAction[30];
    static void stream(const char* action, const char* url);
    static void setup();
    static void loop();
    static void createTask();
};
