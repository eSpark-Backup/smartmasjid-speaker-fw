#pragma once
#include<Arduino.h>
#include <FastLED.h>

#define DATA_PIN     32 // DATA_PIN
#define NUM_LEDS    3
#define BRIGHTNESS 25
#define LED_TYPE    TM1803 // WS2812B is not functioning 
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 1000

typedef enum MODE 
{ 
  BLE, 
  WIFI_CONNECTED,
  WIFI_DISCONNECTED,
  ADHAN_STREAMING, 
  CHARGING_BATTRY,
  LOW_BATTERY,
  RGB_OFF
}MODE;


extern CRGBPalette16 myRedWhiteBluePalette;

void rgbLedTask(void* pvParameters);

class LED
{
  private:
    static CRGBPalette16 currentPalette;
    TBlendType    currentBlending;
    
    

  public:
    CRGB orange;
    CRGB green;
    CRGB red;
    CRGB leds[NUM_LEDS];
    static MODE currentMode;
    static MODE previousMode;

    void setup();
    void showSolidColor(CRGB color);
    void setColor(CRGB color);
    void circleBlinking(CRGB color, const char* speed); // speed can be FAST , MEDIUM and SLOW ,any Argument other than these arguments , the led will not lit
    void turnOffLeds();
    void createTask();
};

extern LED rgb;
