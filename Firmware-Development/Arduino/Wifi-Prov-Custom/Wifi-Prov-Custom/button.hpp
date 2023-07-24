#pragma once
extern bool buttonPressed;
// bool Button ;

#define buttonPin 16

class button 
{
  public:
    void setupfunction();
    static void handleButtonInterrupt();
    bool buttonpressed();
    bool isPressed();
    void clearstate();

};

