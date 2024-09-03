#pragma once
#include "SPIFFS.h"

#define FILE_TITLE "/server.crt"
class SPIFFSHandler 
{
  public:
    void setUp(); 
    void check(); 
    void write(const char* content); 
    void print(); 
    char* get();
};
