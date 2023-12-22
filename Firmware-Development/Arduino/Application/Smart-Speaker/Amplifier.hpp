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
    
    // Set Speaker volume
    uint8_t volume = 5 ;  //default 0.....21
    void start(const char* url = NULL);
    void stop();
    
  public:  
    char currentAction[15];
    void stream(const char* action, const char* url);
    void setup();
    void loop();
};

extern Amplifier amp;