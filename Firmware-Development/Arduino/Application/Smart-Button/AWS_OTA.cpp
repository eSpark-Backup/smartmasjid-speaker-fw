#include"AWS_OTA.hpp"


void AWS_OTA::HttpEvent(HttpEvent_t *event) 
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
  HttpsOTA.onHttpEvent(HttpEvent);
  Serial.println("Starting OTA");
  HttpsOTA.begin(url, server_certificate);
  Serial.println("Please Wait it takes some time ...");
}

bool AWS_OTA::UpdateStart()
{
  otastatus = HttpsOTA.status();
  if (otastatus == HTTPS_OTA_SUCCESS) 
  {
    Serial.println("Firmware written successfully. To reboot device, call API ESP.restart() or PUSH restart button on device");
    return true;
  }  
  else if (otastatus == HTTPS_OTA_FAIL) 
  {
    Serial.println("Firmware Upgrade Fail");
    return false;
  }
  delay(1000);
  return false;
}