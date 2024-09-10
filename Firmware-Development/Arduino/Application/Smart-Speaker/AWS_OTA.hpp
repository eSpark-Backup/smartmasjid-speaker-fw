#pragma once
#include"Arduino.h"
// This sketch provide the functionality of OTA Firmware Upgrade
#include "WiFi.h"
#include "HttpsOTAUpdate.h"

static const char *server_certificate;
static HttpsOTAStatus_t otastatus;
void awsOtaTask(void* pvParameters);

class AWS_OTA
{ 
  public:
   const char* URL;
   bool updated = true;
   static void httpEventHandler(HttpEvent_t *event);
   void setup(const char* url);
   bool updateStart();
   void createTask(const char* url);
};

extern AWS_OTA ota;
