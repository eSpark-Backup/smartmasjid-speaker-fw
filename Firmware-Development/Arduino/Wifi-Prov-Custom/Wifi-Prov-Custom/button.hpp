#pragma once
extern bool buttonPressed;
#define buttonPin 4
#ifndef BUTTON_h
#define BUTTON_h

class Button {
public:
  void setupfunction();
  static void handleButtonInterrupt();

};
#endif


