#pragma once

class SM_Json
{
  public:
    void createFactoryInfoJson(char* jsonString, uint8_t size);  
    void createTelemetryJson(const char* macAddress, int battery, char* json, uint8_t size);
};
