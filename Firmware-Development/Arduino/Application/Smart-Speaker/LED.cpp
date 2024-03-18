#include"LED.hpp"

LED rgb;
CRGBPalette16 LED::currentPalette;

void LED::setColor(CRGB color)
{
    currentPalette = CRGBPalette16(color, color, CRGB::Black,  CRGB::Black);
}

void LED::setup()
{
 FastLED.addLeds<TM1803, DATA_PIN, GRB>(leds, NUM_LEDS);
 FastLED.setBrightness(  BRIGHTNESS );
}

void LED::showSolidColor(CRGB color)
{
  setColor(color);
  uint8_t colorIndex =0, brightness = 255;
  for (int i = 0; i < NUM_LEDS; ++i) 
    {
        leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        FastLED.show();
    }
}

void circleBlinking(CRGB color, Speed speed)
{
    uint8_t brightness = 255;
    uint8_t colorIndex = 1;
    setColor(color);

    for (int i = 0; i < NUM_LEDS; ++i) 
    {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        FastLED.show();

        // Map speed enum to delay time
        int delayTime;
        switch (speed) {
            case FAST: delayTime = 30; break;  // 10000*3/ UPDATES_PER_SECOND
            case MEDIUM: delayTime = 100; break;  // 100000/ UPDATES_PER_SECOND
            case SLOW: delayTime = 500; break;  // 100000*5/ UPDATES_PER_SECOND
            default: delayTime = 0; break;
        }

        if (delayTime > 0)
            FastLED.delay(delayTime);
        else
            setColor(CRGB::Black);
    }
}



