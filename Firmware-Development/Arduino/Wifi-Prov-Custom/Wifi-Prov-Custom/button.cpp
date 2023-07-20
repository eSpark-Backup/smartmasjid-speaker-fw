#include "button.hpp"
#include <Arduino.h>
bool buttonPressed = false;
void handleButtonInterrupt();

void Button::setup()
{
  pinMode(buttonPin, INPUT);
  attachInterrupt(buttonPin, handleButtonInterrupt, RISING);
}
void Button::handleButtonInterrupt() 
{
  buttonPressed = true; // Assuming you have a global variable named buttonPressed
}