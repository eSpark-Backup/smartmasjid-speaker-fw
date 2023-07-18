#pragma once
extern bool buttonPressed;
#define buttonPin 4
#ifndef BUTTON_HPP
#define BUTTON_HPP
class Button {
public:
  void setup();
  static void handleButtonInterrupt();
};
#endif


