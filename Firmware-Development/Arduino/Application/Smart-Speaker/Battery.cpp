#include "Battery.hpp"

BATTERY_STATE state;

void Battery::setUp()
{
  pinMode(BAT_ADC, INPUT);
}

float Battery::getVoltage()
{
  return analogRead(BAT_ADC) * 3.3/4095.0;
}

void Battery::loop()
{
 switch(state)
 {
  case BATTERY_STATE::USB_POWER_PLUGED_IN:
      break;
  case BATTERY_STATE::USB_POWER_PLUGED_OUT:
      break;
  case BATTERY_STATE::LOW_BATERY:
      break;

 }

}

