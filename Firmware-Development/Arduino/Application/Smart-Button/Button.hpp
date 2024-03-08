#pragma once

#include "SM_Http.hpp"

#define SMBTN_GPIO 16

enum buttonState
{
  UNPRESSED ,
  ADHAN_START_REQUESTED,
  ADHAN_STARTED,
  ADHAN_STOP_REQUESTED,
  ADHAN_STOPPED,
  ADHAN_RESET = 10
};

#define TO_MS(seconds) ((seconds) * 1000)
#define ADHAN_MAX_DURATION TO_MS(5*60)

void resetTask(void* pvParameters);

class Button 
{
  private:
    uint8_t pin;
     volatile buttonState state;
    
    void interruptHandler();
    
    uint32_t adhanStartTime= 0;
    
    void createTask();

  public:
    Button(uint8_t pin) : pin(pin) {}
    void setup(); 
    void loop();
    static bool resetFlag; 
    static volatile int counter; 
    static bool isTaskDeleted;

};
extern SmHttp smHttp;
