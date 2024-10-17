#include "WiFi.h"
#include"Amplifier.hpp"
#include "LED.hpp"
#include "Button.hpp"

Audio* audio = NULL;
volatile float Amplifier:: volume = 14; 
bool Amplifier::initilize = false;
Amplifier* Amplifier::instance = nullptr;

Amplifier* Amplifier::getInstance()
{
  if (!instance) 
      {
        instance = new Amplifier();
      }
      return instance;
}

void Amplifier::setup()
{
  if(!initilize)
  {
    audio = new Audio;
    if(!audio->setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT))
    {
      Serial.println("Amplifier is not initilized");
      return;
    }
    initilize = true;
    Serial.println("Amplifier is initilized");
    audio->setVolume(volume);
    Serial.print("volume is set to ");
    Serial.println(volume);
  }
}

void Amplifier::loop()
{
  audio->loop();
}

void Amplifier::start(const char* url)
{
  Serial.println("Amplifier is Started...");
  Serial.print("Streaming from url : ");
  Serial.println(url);
  if(!audio->connecttohost(url))
  { 
    Serial.println("Faild to connect the host");
  }
  else
  {
    Serial.println("connected to  the host");
    xTaskCreatePinnedToCore(amplifier, "amplifier", 15000, NULL, 0, NULL, 0);
    LED& rgb = *LED::getInstance();
    rgb.ADHAN_FLAG = AdhanStatus::STREAMING; 
    rgb.currentMode = MODE::ADHAN_STREAMING;
  }
}

void Amplifier::stop()
{
  Serial.println("Amplifier is Stoped...");
  audio->stopSong();
  LED& rgb = *LED::getInstance();
  rgb.ADHAN_FLAG = AdhanStatus::NO_STREAMING;
}

void Amplifier::stream(const char* action, const char* url)
{
  if (!strcmp(action,"START"))
      start(url);

  else if (!strcmp(action,"STOP"))
      stop();

  strncpy(currentAction, action, strlen(action));  
}

void Amplifier::volumeUp()
{
  volume += 0.5;
  if (volume > MAX_DEFAULT_VOLUME)
    volume = MAX_DEFAULT_VOLUME;
  audio->setVolume(volume);
}

void Amplifier::volumeDown()
{
  if ((volume >= MIN_DEFAULT_VOLUME+1) && (volume <= MAX_DEFAULT_VOLUME))
      volume -= 0.5;
  else if (volume > MAX_DEFAULT_VOLUME)
      volume = MAX_DEFAULT_VOLUME;
  else    
      volume = 0;
  audio->setVolume(volume);
}

void Amplifier::muteVolume()
{
  volume = MIN_DEFAULT_VOLUME;
  audio->setVolume(volume);
}

void Amplifier::volumeLoop()
{
  if (Button::volumeUpFlag) 
  {
    volumeUp();
    Button::volumeUpFlag = false;
    Serial.print("volume :");
    Serial.println(volume);
  }

  if (Button::volumeDownFlag) 
  {
    volumeDown();
    Button::volumeDownFlag = false;
    Serial.print("volume :");
    Serial.println(volume);
  }

  if(Button::volumeMuteFlag)
  {
    muteVolume();
    Button::volumeMuteFlag = false;
    Serial.print("volume :");
    Serial.println(volume);
  }
}

void amplifier(void* pvParameters) 
{
  Amplifier& amp = *Amplifier::getInstance();
  while (1)
  {  
    amp.loop();
    if (!strcmp(amp.currentAction,"STOP"))
    {
      vTaskDelete(NULL);
      break;
    }
  }
  vTaskDelete(NULL);
}
