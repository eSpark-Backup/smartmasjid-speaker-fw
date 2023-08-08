#pragma once

#define SMBTN_GPIO 16

enum buttonState{
  UNPRESSED,
  ADHAN_START_REQUESTED,
};

class Button 
{
  private:
    uint8_t pin;
    volatile buttonState state = UNPRESSED;
    void handleButtonInterrupt();
    
  public:
    Button(uint8_t pin) : pin(pin) {}
    void setup(); 
    void loop();

};
