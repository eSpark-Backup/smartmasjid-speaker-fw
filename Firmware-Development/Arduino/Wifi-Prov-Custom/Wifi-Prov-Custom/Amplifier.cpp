
#include"Amplifier.hpp"

void Amplifier::start(char* url)
{
  Serial.println("Amplifier is Started...");
}

void Amplifier::stop()
{
  Serial.println("Amplifier is Stoped...");
}

void Amplifier::stream(const char* action, char* url)
{
    if (!strcmp(action,"START"))
       start();

    else if (!strcmp(action,"STOP"))
       stop();      
}
