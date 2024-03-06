#include <Arduino.h>
#include "FunctionalInterrupt.h"
#include "Button.hpp"

int Button::counter =-1;
bool Button::resetFlag = false;

void Button::setup()
{
  pinMode(pin, INPUT_PULLDOWN);
  createTask();
  attachInterrupt(pin, std::bind(&Button::interruptHandler, this), CHANGE); 
  
}

void Button::interruptHandler() 
{
 counter ++;
 Button::resetFlag = true;
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
      Serial.print("ADHAN_START_REQUESTED_");
      Serial.println(counter);
      state = ADHAN_STARTED;
      // counter ++;
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
      Serial.print("ADHAN_STOP_REQUESTED_");
      Serial.println(counter);
      counter = 0;
      break;

    default:
      // Serial.print("Unpressed_");
      // Serial.println(counter);
      break;
  }
}

void Button::createTask()
{
  Serial.println("Task Created:");
  xTaskCreatePinnedToCore (resetTask, "resetTask", 1000, this, 0, NULL, 0);
}

void resetTask(void* pvParameters)
{
  while(1)
  {
    if (Button::resetFlag)
    {
      // Check if the button is still pressed after 5 seconds
      vTaskDelay(pdMS_TO_TICKS(5000));// should be update 
      if (digitalRead(SMBTN_GPIO) == LOW)
      {
        Serial.println("Adhan is reset after 5 seconds");
        break;
      }
      // Reset the flag
      Button::resetFlag = false; 
    }
    vTaskDelay(pdMS_TO_TICKS(100)); // Check the button status every 100ms
  }
  Serial.println("Reset Button Task deleted");
  // vTaskDelete(NULL);
  vTaskSuspend(NULL);
}
