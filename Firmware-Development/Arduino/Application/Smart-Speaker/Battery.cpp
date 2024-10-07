#include "Battery.hpp"

BatteryStatus state;

void Battery::setup()
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

BatteryStatus Battery::getStatus()
{
  bool isPlugIn = plugInOutStatus();
  // float batteryTerminalVoltage = getVoltage(); // this should be used instead of the next line
  float batteryTerminalVoltageInPercent = 21; 
  LED& rgb = *LED::getInstance();
  if (rgb.ADHAN_FLAG == AdhanStatus::STREAMING)
    return BatteryStatus::DEFAULT_;

  else if(isPlugIn && batteryTerminalVoltageInPercent <= LOW_BATTERY_THRESHOLD_VOLTAGE)
    return BatteryStatus::USB_POWER_PLUGED_IN;

  else if(batteryTerminalVoltageInPercent <= LOW_BATTERY_THRESHOLD_VOLTAGE)
    return BatteryStatus::LOW_BATERY;

  else if (!isPlugIn)
    return BatteryStatus::USB_POWER_PLUGED_OUT;

  else if(isPlugIn)
    return BatteryStatus::USB_POWER_PLUGED_IN;

  else 
     return BatteryStatus::DEFAULT_;
}

void Battery::loop()
{
  LED& rgb = *LED::getInstance();
  state = getStatus();
  switch(state)
  {
    case BatteryStatus::USB_POWER_PLUGED_IN:
        rgb.currentMode = MODE::BATTRY_IS_CHARGING;
        break;

    case BatteryStatus::USB_POWER_PLUGED_OUT:
        rgb.currentMode = MODE::WIFI_CONNECTED; 
        break;
        
    case BatteryStatus::LOW_BATERY:
        rgb.currentMode = MODE::LOW_BATTERY; 
        break;
  }
}

