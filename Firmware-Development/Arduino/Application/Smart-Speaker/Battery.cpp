#include "Battery.hpp"

BATTERY_STATUS state;

void Battery::setUp()
{
  pinMode(BAT_ADC, INPUT);
  pinMode(BAT_STAT, OUTPUT);
  pinMode(DETECTION, INPUT);
}

float Battery::getVoltage()
{
  return (analogRead(BAT_ADC) * 3.3/4095.0)*100.0/1.78; // returns in percentage 
}

bool Battery::plugInOutStatus()
{
  return !digitalRead(DETECTION);
}

BATTERY_STATUS Battery::Status()
{
  bool isPlugIn = plugInOutStatus();
  // float batteryTerminalVoltage = getVoltage(); // this should be used instead of the next line
  float batteryTerminalVoltageInPercent = 21; 

  if (rgb.ADHAN_FLAG == ADHAN_STATUS::STREAMING)
    return BATTERY_STATUS::DEFAULT_;

  else if(isPlugIn && batteryTerminalVoltageInPercent <= LOW_BATTERY_THRESHOLD_VOLTAGE)
    return BATTERY_STATUS::USB_POWER_PLUGED_IN;

  else if(batteryTerminalVoltageInPercent <= LOW_BATTERY_THRESHOLD_VOLTAGE)
    return BATTERY_STATUS::LOW_BATERY;

  else if (!isPlugIn)
    return BATTERY_STATUS::USB_POWER_PLUGED_OUT;

  else if(isPlugIn)
    return BATTERY_STATUS::USB_POWER_PLUGED_IN;

  else 
     return BATTERY_STATUS::DEFAULT_;
}

void Battery::loop()
{
  state = Status();
  switch(state)
  {
    case BATTERY_STATUS::USB_POWER_PLUGED_IN:
        rgb.currentMode = MODE::BATTRY_IS_CHARGING;
        break;

    case BATTERY_STATUS::USB_POWER_PLUGED_OUT:
        rgb.currentMode = MODE::WIFI_CONNECTED; 
        break;
        
    case BATTERY_STATUS::LOW_BATERY:
        rgb.currentMode = MODE::LOW_BATTERY; 
        break;
  }
}

