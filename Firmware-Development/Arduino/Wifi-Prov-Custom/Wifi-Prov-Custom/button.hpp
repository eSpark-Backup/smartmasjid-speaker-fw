#pragma once
extern bool buttonPressed;
#define buttonPin 4
#ifndef BUTTON_HPP
#define BUTTON_HPP
// #include <httplib.h>
// #include "path/to/httplib.h"
class Button {
public:
  void setup();
  static void handleButtonInterrupt();
//   void button_interrupt_handler();
// };
#endif


