
#include"Amplifier.hpp"

Audio Amplifier::audio;
uint8_t Amplifier::volume = 5;  
char Amplifier::currentAction[30] = "";

void Amplifier::setup()
{
  if(!audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT))
  {
    Serial.println("Amplifier is not initilized");
  }
  Serial.println("Amplifier is initilized");
  audio.setVolume(volume);
  Serial.print("volume is set to ");
  Serial.println(volume);
}

void Amplifier::loop()
{
    audio.loop();
}

void Amplifier::start(const char* url)
{
  Serial.println("Amplifier is Started...");
  Serial.print("Streaming from url : ");
  Serial.println(url);
  audio.connecttohost("https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/Olsen-Banden.mp3");
  loop();
}

void Amplifier::stop()
{
  Serial.println("Amplifier is Stoped...");
  audio.stopSong();
}

void Amplifier::stream(const char* action, const char* url)
{
    if (!strcmp(action,"START"))
       start(url);

    else if (!strcmp(action,"STOP"))
       stop();

    strcpy(currentAction, action);         
}

void Amplifier::createTask()
{
 xTaskCreatePinnedToCore (amplifier, "amplifier", 10000, NULL, 0, NULL, 0);
}

void amplifier(void* pvParameters) 
{
  while (1)
  {    
    if (!strcmp(Amplifier::currentAction,"START"))
    {
      Amplifier::loop();
    }
  }
}
