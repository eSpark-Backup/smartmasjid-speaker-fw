#include <Arduino.h>
#include "FunctionalInterrupt.h"
#include "Button.hpp"

int Button::counter =-1;

void Button::setup()
{
  pinMode(pin, INPUT_PULLDOWN);
  attachInterrupt(pin, std::bind(&Button::interruptHandler, this), RISING); 
}

void Button::interruptHandler() 
{
 counter ++;
//  interruptHandlerStateChange();
}

void Button::interruptHandlerStateChange() 
{ 
  state = (buttonState)counter;
  switch(state)
  {
    case UNPRESSED: 
      // Serial.println("Unpressed"); 
      break;

    case ADHAN_STOP_REQUESTED: 
      // Serial.println("ADHAN_STOP_REQUESTED"); 
      break;

    case ADHAN_STOPPED: 
      // Serial.println("ADHAN_STOPPED"); 
      state = ADHAN_START_REQUESTED;
      counter =0; 
      break; 

    case ADHAN_START_REQUESTED: 
      // Serial.println("ADHAN_START_REQUESTED"); 
      break;
    case ADHAN_STARTED: 
      // Serial.println("ADHAN_START_REQUESTED"); 
      state = ADHAN_STOP_REQUESTED;
      break;

    default:
        // Serial.println("Default"); 
        break;
  }
}

void Button::loop()
{
  state = (buttonState)counter;
  switch(state)
  {
    case ADHAN_START_REQUESTED:
      smHttp.startAdhan();
      adhanStartTime = millis();
      // Serial.print("ADHAN_START_REQUESTED_");
      // Serial.println(counter);
      state = ADHAN_STARTED;
      counter ++;
      break;

    case ADHAN_STARTED:
      if((millis() - adhanStartTime) > ADHAN_MAX_DURATION)
      {
         state = ADHAN_STOP_REQUESTED;
      }
      // Serial.print("ADHAN_STARTED_");
      // Serial.println(counter);
      break;

    case ADHAN_STOP_REQUESTED:
      smHttp.stopAdhan();
      state = ADHAN_STOPPED;
      // Serial.print("ADHAN_STOP_REQUESTED_");
      // Serial.println(counter);
      counter = 0;
      break;

    default:
      // Serial.print("Unpressed_");
      // Serial.println(counter);
      break;
  }
}
