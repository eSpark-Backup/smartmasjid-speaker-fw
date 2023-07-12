#pragma once
#define SM_MODEL "SSB23V1"
#define SM_FIRMWARE_VER "1.0"
#define TESTING

#ifdef TESTING
  #define MAC_ADDRESS "abc:123:efg:456"
#else 
  #define MAC_ADDRESS WiFi.macAddress().c_str()
#endif