
#include"Amplifier.hpp"

void Amplifier::start(const char* url)
{
  Serial.println("Amplifier is Started...");
  Serial.print("Streaming from url : ");
  Serial.println(url);
}

void Amplifier::stop()
{
  Serial.println("Amplifier is Stoped...");
}

void Amplifier::stream(const char* action, const char* url)
{
    if (!strcmp(action,"START"))
       start(url);

    else if (!strcmp(action,"STOP"))
       stop();      
}
