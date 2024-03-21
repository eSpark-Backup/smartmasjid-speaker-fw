
#pragma once
#include "Arduino.h"
#include"LED.hpp"

#define  BAT_ADC  17
#define  BAT_STAT 35
#define  DETECTION 33
#define  LOW_BATTERY_THRESHOLD_VOLTAGE 20  //20 percent is set

typedef enum BATTERY_STATUS
{
  USB_POWER_PLUGED_IN,
  USB_POWER_PLUGED_OUT,
  LOW_BATERY,
  DEFAULT_
} BATTERY_STATUS;

class Battery
{
  private:
    float getVoltage();
    bool plugInOutStatus();
    BATTERY_STATUS Status();

  public:
    void setUp();
    void loop();    
};

