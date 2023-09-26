#pragma once
#include<Arduino.h>

class Amplifier
{
  private:
    void start(char* url = NULL);
    void stop();

  public:  
    void stream(const char* action, char* url);
};
