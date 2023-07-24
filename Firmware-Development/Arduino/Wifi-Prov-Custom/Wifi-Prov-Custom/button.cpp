#include "button.hpp"
#include "HTTP_SM.hpp"
#include <Arduino.h>
bool buttonPressed = false;
void button::setupfunction()
{
  pinMode(buttonPin, INPUT);
  attachInterrupt(buttonPin, handleButtonInterrupt, RISING); 
}
void button::handleButtonInterrupt() 
{
  buttonPressed = true; // Assuming you have a global variable named buttonPressed
}

bool button::isPressed()
{
  return buttonPressed; 
}
void button::clearstate()
{
  buttonPressed= false;
}