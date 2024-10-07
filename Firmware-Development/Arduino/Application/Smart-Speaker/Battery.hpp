
#pragma once
#include "Arduino.h"
#include"LED.hpp"

#define  BAT_ADC  17
#define  BAT_STAT 35
#define  DETECTION 33
#define  LOW_BATTERY_THRESHOLD_VOLTAGE 20  //20 percent is set

typedef enum BatteryStatus
{
  USB_POWER_PLUGED_IN,
  USB_POWER_PLUGED_OUT,
  LOW_BATERY,
  DEFAULT_
} BatteryStatus;

class Battery
{
  private:
    float getVoltage();
    bool plugInOutStatus();
    BatteryStatus getStatus();

  public:
    void setup();
    void loop();    
};

