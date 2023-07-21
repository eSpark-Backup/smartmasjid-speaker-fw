#include "button.hpp"
#include <Arduino.h>
bool buttonPressed = False;
// void handleButtonInterrupt()
void Button::setupfunction()
{
  pinMode(buttonPin, INPUT);
  attachInterrupt(buttonPin, handleButtonInterrupt, RISING);
  
}
void Button::handleButtonInterrupt() 
{
  buttonPressed = true; // Assuming you have a global variable named buttonPressed
}
