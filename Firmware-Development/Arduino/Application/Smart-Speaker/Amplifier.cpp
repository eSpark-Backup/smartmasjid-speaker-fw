#include "WiFi.h"
#include"Amplifier.hpp"

Amplifier amp; 
Audio* audio = NULL;

void Amplifier::setup()
{
  Serial.print("Amplifier");
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
  //Serial.print("L");
  audio->loop();
}
void Amplifier::start(const char* url)
{
  Serial.println("Amplifier is Started...");
  Serial.print("Streaming from url : ");
  Serial.println(url);
  delay(10);
  if(!audio->connecttohost(url))
  { 
    Serial.println("Faild to connect the host");
  }
  else
    Serial.println("connected to  the host");

  xTaskCreatePinnedToCore (amplifier, "amplifier", 12000, NULL, 0, NULL, 1);
}

void Amplifier::stop()
{
  Serial.println("Amplifier is Stoped...");
  audio->stopSong();
}

void Amplifier::stream(const char* action, const char* url)
{
  if (!strcmp(action,"START"))
      start(url);

  else if (!strcmp(action,"STOP"))
      stop();

  strncpy(currentAction, action, strlen(action));  

}

void Amplifier::createTask()
{
  Serial.println ("Task#Created");
  // xTaskCreatePinnedToCore (amplifier, "amplifier", 10000, NULL, 0, NULL, 0);
}

void amplifier(void* pvParameters) 
{
    int coreID = xPortGetCoreID();
    Serial.print("Current Core ID: ");
    Serial.println(coreID);
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
