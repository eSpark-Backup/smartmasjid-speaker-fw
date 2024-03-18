#pragma once
#include<Arduino.h>
#include <FastLED.h>

#define DATA_PIN     32 // DATA_PIN
#define NUM_LEDS    3
#define BRIGHTNESS 25
#define LED_TYPE    TM1803 // WS2812B is not functioning 
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 1000

enum Speed 
{ 
  FAST, 
  MEDIUM, 
  SLOW 
};


extern CRGBPalette16 myRedWhiteBluePalette;

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

    void setup();
    void showSolidColor(CRGB color);
    void setColor(CRGB color);
    void circleBlinking(CRGB color, const char* speed); // speed can be FAST , MEDIUM and SLOW other than these argument , the led will not lit
};

extern LED rgb;
