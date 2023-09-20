
#include"Amplifier.hpp"

void Amplifier::start()
{
  Serial.println("Amplifier is Started...");
}

void Amplifier::stop()
{
  Serial.println("Amplifier is Stoped...");
}

void Amplifier::stream(const char* action)
{
    if (strcmp(action,"START")==0)
       start();

    else
       stop();      
}

Adhan Amplifier::getAdhan(char* adhan) 
{
  if (strcmp(adhan, "fajr") == 0) 
    return FAJR;
  
  if (strcmp(adhan, "dhuhr") == 0) 
    return DHUHR;
  
  if (strcmp(adhan, "asr") == 0) 
    return ASR;
  
  if (strcmp(adhan, "maghrib") == 0) 
    return MAGHRIB;
  
  if (strcmp(adhan, "isha") == 0)
    return ISHA;

  if (strcmp(adhan, "jummah1") == 0)
    return JUMMAH1;

  if (strcmp(adhan, "jummah2") == 0)
    return JUMMAH2;

  if (strcmp(adhan, "jummah3") == 0)
    return JUMMAH3;
}

void Amplifier::adhanStream(char* adhan, char*action)
{
  Adhan topic = getAdhan(adhan);
  switch (topic)
  {
    case FAJR:
      Serial.print("fajr\t");
      Serial.println(action);
      stream(action);
      break;

    case DHUHR:
      Serial.print("dhuhur\t");
      Serial.println(action);
      stream(action);
      break;

    case ASR:
      Serial.print("Aser\t");
      Serial.println(action);
      stream(action);
      break;

    case MAGHRIB:
      Serial.print("Magrib\t");
      Serial.println(action);
      stream(action);
      break;

    case ISHA:
      Serial.print("Isha\t");
      Serial.println(action);
      stream(action);
      break;

    case JUMMAH1:
      Serial.print("jummah1\t");
      Serial.println(action);
      stream(action);
      break;

    case JUMMAH2:
      Serial.print("jummah2\t");
      Serial.println(action);
      stream(action);
      break;

    case JUMMAH3:
      Serial.print("jummah3\t");
      Serial.println(action);
      stream(action);
      break;

    default:
      Serial.println("UNKNOWN ADHAN");       
  }
}
