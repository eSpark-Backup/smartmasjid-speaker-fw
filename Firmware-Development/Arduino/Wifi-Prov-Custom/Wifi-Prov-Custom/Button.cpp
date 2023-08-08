#include <Arduino.h>
#include "FunctionalInterrupt.h"
#include "SM_Http.hpp"
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
      state = ADHAN_START_REQUESTED;
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
      {
        SmHttp smHttp;
        smHttp.startAdhan();
      }
      state = UNPRESSED;
      break;

    default:
      break;
  }
}
