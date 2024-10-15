#include"Button.hpp"
#include"Amplifier.hpp"
#include"Device.hpp"

//volatile byte Button::stateUpButtonLed = LOW;
//volatile byte Button::stateDownButtonLed = LOW;
//volatile byte Button::stateMuteButtonLed = LOW;
//volatile byte Button::stateResetButtonLed = LOW;

const uint32_t Button::PRESS_DURATION;
bool Button::resetButtonPressed = false;
MQTTClient* Button::pMqtt = nullptr;

void Button::setup(MQTTClient* mqtt)
{
  pMqtt = mqtt;

  pinMode (UP_BUTTON, INPUT);
  pinMode (DOWN_BUTTON, INPUT);
//  pinMode (MUTE_BUTTON, INPUT);
  // pinMode (RESET_BUTTON, INPUT);


  attachInterrupt(digitalPinToInterrupt(UP_BUTTON), Button::volumeUpWrapper, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DOWN_BUTTON), Button::volumeDownWrapper, CHANGE);
//  attachInterrupt(digitalPinToInterrupt(MUTE_BUTTON), Button::volumeMuteWrapper, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(RESET_BUTTON), Button::resetButtonISR, CHANGE); 
}


void Button::volumeUpWrapper() 
{
  Amplifier& amp = *Amplifier::getInstance();
  amp.volumeUp();
//  Button::stateUpButtonLed = !Button::stateUpButtonLed;
}

void Button::volumeDownWrapper() 
{
  Amplifier& amp = *Amplifier::getInstance();
  amp.volumeDown();
//  Button::stateDownButtonLed = !Button::stateDownButtonLed;
}

void Button::volumeMuteWrapper() 
{
  Amplifier& amp = *Amplifier::getInstance();
  amp.muteVolume();
//  Button::stateMuteButtonLed = !Button::stateMuteButtonLed;
}

//// Will not work as expected
void Button::resetButtonTask(void *parameter)
{
  while(1)
  {
    if (resetButtonPressed)
    {
      // Check if the button is still pressed after 5 seconds
      vTaskDelay(pdMS_TO_TICKS(PRESS_DURATION));
      if (digitalRead(RESET_BUTTON) == LOW)
      {
        Serial.println("Device is reset after 5 seconds");
        pMqtt->unsubscribeTopics();
        Device::deleteSubscriptions();
        Device::deleteWificredentials();
        ESP.restart();
        break;
      }
      // Reset the flag
      resetButtonPressed = false; 
    }
    vTaskDelay(pdMS_TO_TICKS(100)); // Check the button status every 100ms
  }
  Serial.println("Reset Button Task deleted");
  vTaskDelete(NULL);
}

void IRAM_ATTR Button::resetButtonISR()
{
  // Create the reset button task
  xTaskCreate(
      resetButtonTask,   // Task function
      "ResetButtonTask", // Task name
      2000,              // Stack size (bytes)
      NULL,              // Task parameters
      1,                 // Priority (0 is the lowest priority)
      NULL               // Task handle
  );
  // Set the flag indicating that the reset button is pressed
  resetButtonPressed = true;
  Button::stateResetButtonLed = !Button::stateResetButtonLed;
}
