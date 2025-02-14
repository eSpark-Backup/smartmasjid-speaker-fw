#include "LED.hpp"
#include <cstring>

LED rgb;
CRGBPalette16 LED::currentPalette;
MODE LED::currentMode = MODE::RGB_OFF;
AdhanStatus LED::ADHAN_FLAG = AdhanStatus::NO_STREAMING;

LED* LED::instance = nullptr;

LED* LED::getInstance()
{
  if (!instance) 
      {
        instance = new LED();
      }
      return instance;
}

void LED::setColor(CRGB color)
{
    currentPalette = CRGBPalette16(color, color, CRGB::Black, CRGB::Black);
    FastLED.setBrightness(BRIGHTNESS);
}

void LED::setup()
{
    FastLED.addLeds<TM1803, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
}

void LED::showSolidColor(CRGB color)
{

    FastLED.setBrightness(BRIGHTNESS);
    fill_solid(leds, NUM_LEDS, color);
    FastLED.show();
}

void LED::showBlinkingColor(CRGB color, uint16_t fadeDelay)
{
    for (int i = 0; i <= BRIGHTNESS; ++i) {
        FastLED.setBrightness(i);
        fill_solid(leds, NUM_LEDS, color);
        FastLED.show();
        delay(fadeDelay); 
    }
    for (int i = BRIGHTNESS; i >= 0; --i) {
        FastLED.setBrightness(i);
        fill_solid(leds, NUM_LEDS, color);
        FastLED.show();
        delay(fadeDelay);
    } 
}

void LED::circleBlinking(CRGB color, const char *speed)
{
    uint8_t brightness = 255;
    uint8_t colorIndex = 1;
    CRGB black = CRGB::Black;
    CRGBPalette16 currentPalette_Black = black;
    // currentPalette = CRGBPalette16(color, color, CRGB::Black,  CRGB::Black);
    setColor(color);

    for (int i = 0; i < NUM_LEDS; ++i)
    {
        // Turn off all LEDs
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        // Turn on the current LED
        leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        FastLED.show();

        // if (speed == "FAST")
        if (strcmp(speed, "FAST") == 0)
            FastLED.delay(10000 * 3 / UPDATES_PER_SECOND);
        // else if (speed == "MEDIUM")
        else if (strcmp(speed, "MEDIUM") == 0)
            FastLED.delay(100000 / UPDATES_PER_SECOND);
        // else if (speed == "SLOW")
        else if (strcmp(speed, "SLOW") == 0)
            FastLED.delay(100000 * 5 / UPDATES_PER_SECOND);
        else
        {
            setColor(black);
        }
    }
}

void LED::turnOffLeds()
{
    FastLED.setBrightness(0);
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
}

void LED::createTask()
{
    xTaskCreatePinnedToCore(rgbLedTask, "rgbLedTask", 6000, this, 0, NULL, 0);
}

void rgbLedTask(void *pvParameters)
{
    // delay(3000);
    rgb.setup();
    rgb.orange = CHSV(HUE_ORANGE, 255, 255);
    rgb.red = CHSV(HUE_RED, 255, 255);
    rgb.green = CHSV(HUE_GREEN, 255, 255);
    rgb.white = CHSV(0, 0, 255); // White color

    // rgb.turnOffLeds();

    while (1)
    {
        switch (LED::currentMode)
        {
            case MODE::BLE:
                rgb.circleBlinking(rgb.orange, "MEDIUM"); // LED behavior is Orange circle blinking at medium speed
                break;

            case MODE::WIFI_CONNECTED:
                rgb.showSolidColor(rgb.orange); // the board gets connected to WiFi but is sitting idle
                break;

            case MODE::ADHAN_STREAMING:
                rgb.circleBlinking(rgb.green, "SLOW"); // LED behavior should be Green circle blinking slowly
                break;

            case MODE::BATTRY_IS_CHARGING:
                rgb.showSolidColor(rgb.green); // It should show Green light constantly if USB power is plugged in
                break;

            case MODE::LOW_BATTERY:
                rgb.showSolidColor(rgb.red); // LED behavior is Red constant only when there is no USB charger plugged in
                break;
            case MODE::ON_POWER_UP:
                rgb.showBlinkingColor(rgb.white, FADE_DURATION); // LED behavior should be Green circle blinking slowly
                break;

            default:
                rgb.turnOffLeds();
                break;
        }
    }

    vTaskDelete(NULL);
}
