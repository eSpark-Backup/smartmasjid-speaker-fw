#include <Arduino.h>
#include "Button.hpp"

bool buttonPressed = false;

void Button::setup()
{
  pinMode(SMBTN_GPIO, INPUT);
  attachInterrupt(SMBTN_GPIO, handleButtonInterrupt, RISING); 
}

void Button::handleButtonInterrupt() 
{
  buttonPressed = true;
}

bool Button::isPressed()
{
  return buttonPressed; 
}

void Button::clearState()
{
  buttonPressed= false;
}
