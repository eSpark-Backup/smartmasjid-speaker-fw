#pragma once
#include<Arduino.h>

typedef enum
{
  FAJR = 1,
  DHUHR,
  ASR,
  MAGHRIB,
  ISHA,
  JUMMAH1,
  JUMMAH2,
  JUMMAH3,
} Adhan;

class Amplifier
{
  private:
    void start();
    void stop();
    void stream(const char* action, char* streamUrl);
    Adhan getAdhan (char* adhan);

  public:  
    void adhanStream(char* adhan, char*action);
};