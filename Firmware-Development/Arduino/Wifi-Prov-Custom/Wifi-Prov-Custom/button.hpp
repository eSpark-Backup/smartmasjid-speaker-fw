#pragma once

#define SMBTN_GPIO 16

class Button 
{
  public:
    void setup();
    static void handleButtonInterrupt();
    bool isPressed();
    void clearState();

};
