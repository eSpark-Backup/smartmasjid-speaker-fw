#pragma once

#include "SM_Http.hpp"

#define SMBTN_GPIO 16

enum buttonState
{
  UNPRESSED,
  ADHAN_START_REQUESTED,
  ADHAN_STARTED,
  ADHAN_STOP_REQUESTED,
  ADHAN_STOPPED
};

#define TO_MS(seconds) ((seconds) * 1000)
#define ADHAN_MAX_DURATION TO_MS(5*60)

class Button 
{
  private:
    uint8_t pin;
    volatile buttonState state = UNPRESSED;
    SmHttp smHttp;
    void interruptHandler();
    
    uint32_t adhanStartTime= 0;
    static int counter;
    
  public:
    Button(uint8_t pin) : pin(pin) {}
    void interruptHandlerStateChange();
    void setup(); 
    void loop();

};
