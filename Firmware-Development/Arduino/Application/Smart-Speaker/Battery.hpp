
#pragma once
#include "Arduino.h"

#define  BAT_ADC  17
#define  BAT_STAT 35
#define  DETECTION 33

typedef enum BATTERY_STATE
{
  USB_POWER_PLUGED_IN,
  USB_POWER_PLUGED_OUT,
  LOW_BATERY
} BATTERY_STATE;

class Battery
{
  private:
    float voltage = 0.0;

  public:
    void setUp();
    float getVoltage();
    void loop();

    
};

