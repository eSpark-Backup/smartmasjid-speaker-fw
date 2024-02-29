#include"Button.hpp"
#include"Amplifier.hpp"

volatile byte Button::stateUpButtonLed = LOW;
volatile byte Button::stateDownButtonLed = LOW;
volatile byte Button::stateMuteButtonLed = LOW;
volatile byte Button::stateResetButtonLed = LOW;

const uint8_t Button::RESET_BUTTON;
const uint32_t Button::LONG_PRESS_DURATION;
bool Button::resetButtonPressed = false;

void Button::setUp()
{
  pinMode (UP_BUTTON, INPUT);
  pinMode (DOWN_BUTTON, INPUT);
  pinMode (MUTE_BUTTON, INPUT);
  // pinMode (RESET_BUTTON, INPUT);

  pinMode(UP_BUTTON_LED_1, OUTPUT);//led
  pinMode(DOWN_BUTTON_LED_2, OUTPUT);//led
  pinMode(MUTE_BUTTON_LED_3, OUTPUT);//led
  // pinMode(RESET_BUTTON_LED, OUTPUT);//led
  // pinMode(19, OUTPUT);//led

  attachInterrupt(digitalPinToInterrupt(UP_BUTTON), Button::volumeUpWrapper, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DOWN_BUTTON), Button::volumeDownWrapper, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(MUTE_BUTTON), Button::volumeMuteWrapper, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(RESET_BUTTON), Button::resetWrapper, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RESET_BUTTON), resetButtonISR, FALLING);

  // Create the reset button task
  xTaskCreate(
      resetButtonTask,   // Task function
      "ResetButtonTask", // Task name
      1000,              // Stack size (bytes)
      NULL,              // Task parameters
      1,                 // Priority (0 is the lowest priority)
      NULL               // Task handle
  );
}

void Button::loop()
{
  digitalWrite(UP_BUTTON_LED_1, !Button::stateUpButtonLed);
  digitalWrite(DOWN_BUTTON_LED_2, !Button::stateDownButtonLed);
  // digitalWrite(MUTE_BUTTON_LED_3, !Device::stateMuteButtonLed);
  // digitalWrite(RESET_BUTTON_LED_4, !Device::stateResetButtonLed);
}

void Button::volumeUpWrapper() 
{
  amp.volumeUp();
  Button::stateUpButtonLed = !Button::stateUpButtonLed;
}

void Button::volumeDownWrapper() 
{
  amp.volumeDown();
  Button::stateDownButtonLed = !Button::stateDownButtonLed;
}

void Button::volumeMuteWrapper() 
{
  amp.muteVolume();
  Button::stateMuteButtonLed = !Button::stateMuteButtonLed;
}

void Button::resetButtonTask(void *parameter)
{
  while(1)
  {
    if (resetButtonPressed)
    {
      // Check if the button is still pressed after 5 seconds
      vTaskDelay(pdMS_TO_TICKS(LONG_PRESS_DURATION));
      if (digitalRead(RESET_BUTTON) == LOW)
      {
        Serial.println("Device is reset after 5 seconds");
        // Perform reset actions here
      }

      // Reset the flag
      resetButtonPressed = false;
    }

    vTaskDelay(pdMS_TO_TICKS(100)); // Check the button status every 100ms
  }
}

void IRAM_ATTR Button::resetButtonISR()
{
  // Set the flag indicating that the reset button is pressed
  resetButtonPressed = true;
}

void Button::resetButtonPressedHandler()
{
  // This function is empty and can be implemented to handle other actions when the reset button is pressed
}


