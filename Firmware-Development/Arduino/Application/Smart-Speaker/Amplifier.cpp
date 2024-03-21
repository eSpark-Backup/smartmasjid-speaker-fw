#include "WiFi.h"
#include"Amplifier.hpp"
#include "LED.hpp"

Amplifier amp; 
Audio* audio = NULL;
uint8_t Amplifier:: volume = 5; 

void Amplifier::setup()
{
  audio = new Audio;
  if(!audio->setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT))
  {
    Serial.println("Amplifier is not initilized");
  }
  else
    Serial.println("Amplifier is initilized");
  audio->setVolume(volume);
  Serial.print("volume is set to ");
  Serial.println(volume);
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
  // if(!audio->connecttohost(url))
  if(false)
  { 
    Serial.println("Faild to connect the host");
  }
  else
  {
    Serial.println("connected to  the host");
    xTaskCreatePinnedToCore(amplifier, "amplifier", 10000, NULL, 0, NULL, 0);
    rgb.ADHAN_FLAG = ADHAN_STATUS::STREAMING; 
    rgb.currentMode = MODE::ADHAN_STREAMING;
  }
}

void Amplifier::stop()
{
  Serial.println("Amplifier is Stoped...");
  audio->stopSong();

  rgb.ADHAN_FLAG = ADHAN_STATUS::NO_STREAMING;
  // rgb.currentMode = rgb.turnOffLeds();
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
  volume++;
  if (volume > MAX_DEFAULT_VOLUME)
    volume = MAX_DEFAULT_VOLUME;
  audio->setVolume(volume);
  // Serial.println("Volume : up");
  // Serial.println(volume);
  // delay(100);
}

void Amplifier::volumeDown()
{

  if (volume >= (MIN_DEFAULT_VOLUME+1) && volume <= MAX_DEFAULT_VOLUME)
      volume--;
  else if (volume > MAX_DEFAULT_VOLUME)
      volume = MAX_DEFAULT_VOLUME;
  else    
       volume = 0;
  audio->setVolume(volume);
  // Serial.println("Volume : down");
  // Serial.println(volume);
  // delay(100);
}

void Amplifier::muteVolume()
{
  volume = MIN_DEFAULT_VOLUME;
  audio->setVolume(volume);
  // Serial.println("Volume mute : ");
  // Serial.println(volume);
  // delay(100);
}

void amplifier(void* pvParameters) 
{
  while (1)
  {  
      amp.loop();
      if (!strcmp(amp.currentAction,"STOP"))
      {
          break;
      }
  }
  vTaskDelete(NULL);
}