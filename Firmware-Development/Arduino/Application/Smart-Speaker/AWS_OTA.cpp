#include"AWS_OTA.hpp"
#include "SM_Http.hpp"
#include"crtSpiff.hpp"

AWS_OTA ota;
#define OTA_ROLL_BACK "R"
#define OTA_SUCCESS "C"

void AWS_OTA::httpEventHandler(HttpEvent_t *event) 
{
  switch (event->event_id) 
  {
    case HTTP_EVENT_ERROR:        Serial.println("Http Event Error"); break;
    case HTTP_EVENT_ON_CONNECTED: Serial.println("Http Event On Connected"); break;
    case HTTP_EVENT_HEADER_SENT:  Serial.println("Http Event Header Sent"); break;
    case HTTP_EVENT_ON_HEADER:    Serial.printf("Http Event On Header, key=%s, value=%s\n", event->header_key, event->header_value); break;
    case HTTP_EVENT_ON_DATA:      break;
    case HTTP_EVENT_ON_FINISH:    Serial.println("Http Event On Finish"); break;
    case HTTP_EVENT_DISCONNECTED: Serial.println("Http Event Disconnected"); break;
    // case HTTP_EVENT_REDIRECT:     Serial.println("Http Event Redirect"); break;
  }
}

void AWS_OTA::setUp(const char* url)
{
  SPIFFSHandler spiffsHandler;
  spiffsHandler.setUp();
  spiffsHandler.check();
  //spiffsHandler.write(server_certificate);  // Write new content to the file
  //url = spiffsHandler.get();
  server_certificate = spiffsHandler.get();  // Get the content of the file as a char array
  if (server_certificate) 
  {
    Serial.print("server_certificate content via get(): ");
    Serial.println(server_certificate);
    //delete[] server_certificate;  
  }

  HttpsOTA.onHttpEvent(httpEventHandler);
  Serial.println("Starting OTA");
  HttpsOTA.begin(url, server_certificate);
  Serial.println("Please Wait it takes some time ...");
}

bool AWS_OTA::UpdateStart()
{
  otastatus = HttpsOTA.status();
  Serial.print("Ota Status = ");
  Serial.println(otastatus);
  if (otastatus == HTTPS_OTA_SUCCESS) 
  {
    Serial.println("Firmware written successfully. To reboot device, call API ESP.restart() or PUSH restart button on device");
    OTA_UPDATED = true;
    return true;
  }  
  else if (otastatus == HTTPS_OTA_FAIL) 
  {
    Serial.println("Firmware Upgrade Fail");
    // return false;
  }
    else if (otastatus == HTTPS_OTA_UPDATING) 
  {
    Serial.println("HTTPS OTA is UPDATING");
  }
  else if (otastatus == HTTPS_OTA_IDLE) 
  {
    Serial.println("HTTPS OTA is IDLE");
  }
  delay(1000);

  return false;
}

void AWS_OTA::createTask(const char* url)
{
  // size_t len = strlen(url) + 1;
  // URL = new char[len];
  // memcpy(URL, url, len);
  URL = url;
  Serial.println(URL);
  xTaskCreatePinnedToCore (awsOtaTask, "awsOtaTask", 8000, this, 20, NULL, 1);
}

void awsOtaTask(void* pvParameters) 
{
  ota.setUp(ota.URL);
  SmHttp http;
  while(1)
  {
    ota.UpdateStart();
    if(ota.OTA_UPDATED)
        {
          SmHttp http;
          if(http.notifyOTAUpdate(Device::macAddress))
          {
            Serial.println("Device update reported successfully");
            http.put(Device::macAddress, OTA_SUCCESS);
            ESP.restart();
          }
          http.put(Device::macAddress, OTA_ROLL_BACK);
          ota.OTA_UPDATED = false;
          delete ota.URL;
          ota.URL = nullptr;
          Serial.print("TAsk Deleted +++++++++++\n");
          vTaskDelete( NULL );
        }
  }
}