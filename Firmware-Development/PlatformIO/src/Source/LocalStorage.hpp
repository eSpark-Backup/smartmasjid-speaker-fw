#pragma once
#include "SPIFFS.h"
#include "ArduinoJson.h"

#define FILE_TITLE "/server.crt"
#define FILE_JSON "/config.json"
class LocalStorage 
{
  public:
    void setup(); 
    void isExist(); 
    void write(const char* content); 
    void print(); 
    char* get(const char* filename);
    void deleteFile();
};

extern LocalStorage localStorage;