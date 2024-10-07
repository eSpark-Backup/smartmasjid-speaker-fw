#pragma once
#include <Arduino.h>
#include "Audio.h"

//MAX98357AETE+T Amplifier IC pin
#define I2S_DOUT     25
#define I2S_BCLK     27
#define I2S_LRC      26

//default value of volume 
#define DEFAULT_VOLUME       5
#define MAX_DEFAULT_VOLUME   21
#define MIN_DEFAULT_VOLUME   0 

//amplifier task prototype
void amplifier(void* pvParameters);

class Amplifier
{
  private:
    Amplifier() {}
    // Set Speaker volume
    static uint8_t volume;  //default 0.....21
    static bool initilize;  //Initilization status
    void start(const char* url = NULL);
    void stop();
    
  public:  

    Amplifier(const Amplifier&) = delete;
    Amplifier& operator=(const Amplifier&) = delete;
    static Amplifier* getInstance(); 
    static Amplifier* instance;
    char currentAction[15];
    void stream(const char* action, const char* url);
    void setup();
    void loop();
    void volumeUp();
    void volumeDown();
    void muteVolume();
};

