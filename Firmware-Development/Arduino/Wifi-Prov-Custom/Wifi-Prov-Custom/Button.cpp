#include <Arduino.h>
#include "FunctionalInterrupt.h"
#include "Button.hpp"

void Button::setup()
{
  pinMode(pin, INPUT);
  attachInterrupt(pin, std::bind(&Button::handleButtonInterrupt, this), RISING); 
}

void Button::handleButtonInterrupt() 
{
  switch(state)
  {
    case UNPRESSED:
    case ADHAN_STOP_REQUESTED:
    case ADHAN_STOPPED:
      state = ADHAN_START_REQUESTED;
      break;

    case ADHAN_START_REQUESTED:
    case ADHAN_STARTED:
      state = ADHAN_STOP_REQUESTED;
      break;

    default:
        break;
  }
}

void Button::loop()
{
  switch(state)
  {
    case ADHAN_START_REQUESTED:
      smHttp.startAdhan();
      adhanStartTime = millis();
      state = ADHAN_STARTED;
      break;

    case ADHAN_STARTED:
      if((millis() - adhanStartTime) > ADHAN_MAX_DURATION)
      {
         state = ADHAN_STOP_REQUESTED;
      }
      break;

    case ADHAN_STOP_REQUESTED:
      smHttp.stopAdhan();
      state = ADHAN_STOPPED;
      break;

    default:
      break;
  }
}
