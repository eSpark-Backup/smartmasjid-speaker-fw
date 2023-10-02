#pragma once
#include<Arduino.h>

class Amplifier
{
  private:
    void start(const char* url = NULL);
    void stop();

  public:  
    void stream(const char* action, const char* url);
};
