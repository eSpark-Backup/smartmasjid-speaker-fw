#include <Arduino.h>
#include "FunctionalInterrupt.h"
#include "Button.hpp"

volatile int Button::counter = -1;
bool Button::resetFlag = false;
bool Button::isTaskDeleted = false;
extern volatile buttonState state = UNPRESSED;


void Button::setup()
{
  pinMode(pin, INPUT_PULLDOWN);
  createTask();
  attachInterrupt(pin, std::bind(&Button::interruptHandler, this), RISING);  
}

void Button::interruptHandler() 
{
 Button::counter++;
 Button::resetFlag = true;
}

void Button::loop()
{
  state = (buttonState)counter;
  // Serial.print("+++++++++++++++++++++++++++++++++++++++++=======================");
  // Serial.println(Button::counter);
  switch(state)
  {
    case ADHAN_START_REQUESTED:
      smHttp.startAdhan();
      adhanStartTime = millis();
      Serial.print("ADHAN_START");
      state = ADHAN_STARTED;
      if (Button::isTaskDeleted)
        {
          createTask();
          Button::isTaskDeleted = false;
        }
      Button::counter ++;
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
      Serial.print("ADHAN_STOP");
      Button::counter = 0;
      break;

    case ADHAN_RESET:
      smHttp.resetAdhan();
      Serial.print("ADHAN_RESET");
      Button::counter = 0;
      break;

    default:
      break;
      }
}

void Button::createTask()
{
  Serial.println("\nReset Button Task Created.");
  xTaskCreatePinnedToCore (resetTask, "resetTask", 4000, this, 17, NULL, 0);
}

void resetTask(void* pvParameters)
{
  TickType_t buttonPressStartTime = 0;
  while(1)
  {
    if (digitalRead(SMBTN_GPIO) == LOW)
    {
      if (buttonPressStartTime == 0)
      {
        buttonPressStartTime = xTaskGetTickCount();
      }
      else if ((xTaskGetTickCount() - buttonPressStartTime) >= pdMS_TO_TICKS(5000))
      {
         Serial.println("\nAdhan reset API called after 5 seconds of button hold\n\n");
         Button::resetFlag = true;
         state = ADHAN_RESET;
         Button::counter = 10;
         break;
      }
    }
    else
    {
      buttonPressStartTime = 0;
    }

    vTaskDelay(pdMS_TO_TICKS(100)); 
  }

  Serial.println("Reset Button Task Deleted.\n\n");
  Button::isTaskDeleted = true;
  vTaskDelete(NULL);
}
