#include "Button.hpp"
#include "Amplifier.hpp"
#include "Device.hpp"

volatile bool Button::volumeUpFlag = false;
volatile bool Button::volumeDownFlag = false;
volatile bool Button::volumeMuteFlag = false;

const uint32_t Button::PRESS_DURATION;
bool Button::resetButtonPressed = false;
MQTTClient* Button::pMqtt = nullptr;

void Button::setup(MQTTClient* mqtt)
{
  pMqtt = mqtt;

  pinMode (UP_BUTTON, INPUT_PULLUP);
  pinMode (DOWN_BUTTON, INPUT_PULLUP);
  // pinMode (MUTE_BUTTON, INPUT_PULLUP);
  pinMode (RESET_BUTTON, INPUT_PULLUP);

  Button::interruptHandler();
}


void Button::volumeUpWrapper() 
{
 volumeUpFlag = true;
}

void Button::volumeDownWrapper() 
{
  volumeDownFlag = true;
}

void Button::volumeMuteWrapper() 
{
  volumeMuteFlag = true;
}

void Button::resetButtonTask(void *parameter)
{
  TickType_t pressStartTime = 0; 
  bool isButtonHeld = false;      

  while (1)
  {
    if (resetButtonPressed)
    {
      if (!isButtonHeld)
      {
        pressStartTime = xTaskGetTickCount();  
        isButtonHeld = true; 
      }

      if (digitalRead(RESET_BUTTON) == LOW)
      {
        Serial.println("Device is reset a4 seconds");

        if ((xTaskGetTickCount() - pressStartTime) >= pdMS_TO_TICKS(PRESS_DURATION))
        {
          Serial.println("Device is reset after 5 seconds");
          pMqtt->unsubscribeTopics();
          Device::deleteSubscriptions();
          Device::deleteWificredentials();
          ESP.restart();  // Restart the device
          break;
        }
      }
      else
      {
        resetButtonPressed = false;
        isButtonHeld = false; 
      }
    }
    vTaskDelay(pdMS_TO_TICKS(10)); 
  }

  Serial.println("Reset Button Task deleted");
  vTaskDelete(NULL);  
}

void IRAM_ATTR Button::resetButtonISR()
{
  if (!resetButtonPressed) 
  {
    xTaskCreate(resetButtonTask,"ResetButtonTask", 2000, NULL, 0, NULL);
    resetButtonPressed = true;
    Serial.println("Reset button ISR triggered");
  }
}



void Button::interruptHandler()
{
  attachInterrupt(digitalPinToInterrupt(UP_BUTTON), Button::volumeUpWrapper, FALLING);
  attachInterrupt(digitalPinToInterrupt(DOWN_BUTTON), Button::volumeDownWrapper, FALLING);
  // attachInterrupt(digitalPinToInterrupt(MUTE_BUTTON), Button::volumeMuteWrapper, FALLING);
  attachInterrupt(digitalPinToInterrupt(RESET_BUTTON), Button::resetButtonISR, FALLING);
}
