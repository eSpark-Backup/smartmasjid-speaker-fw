#include"AWS_OTA.hpp"
#include "SM_Http.hpp"
#include"CertificateHandler.hpp"
#include "Device.hpp"

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

void AWS_OTA::setup(const char* url)
{
  HttpsOTA.onHttpEvent(httpEventHandler);
  Serial.println("Starting OTA");
  HttpsOTA.begin(URL, server_certificate);
  Serial.println("Please Wait it takes some time ...");
}

bool AWS_OTA::updateStart()
{
  otastatus = HttpsOTA.status();
  Serial.print("Ota Status = ");
  Serial.println(otastatus);
  if (otastatus == HTTPS_OTA_SUCCESS) 
  {
    Serial.println("Firmware written successfully. To reboot device, call API ESP.restart() or PUSH restart button on device");
    return true;
  }  
  else if (otastatus == HTTPS_OTA_FAIL) 
  {
    Serial.println("Firmware Upgrade Fail");
    updated = false;
    return false;
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
  URL = url;
  Serial.println(URL);
  
  CertificateHandler certificateHandler;
  certificateHandler.setup();
  certificateHandler.isExist();
  //certificateHandler.write(server_certificate);  // Write new content to the file
  server_certificate = certificateHandler.get();  // Get the content of the file as a char array
  if (server_certificate) 
  {
    Serial.print("server_certificate content via get(): ");
    Serial.println(server_certificate);
    //delete[] server_certificate;  
  }
  Device::otaUpdating = true;
  xTaskCreatePinnedToCore (awsOtaTask, "awsOtaTask", 10000, this, 20, NULL, 1);
}

void awsOtaTask(void* pvParameters) 
{
  ota.setup(ota.URL);
  SmHttp http;

  while (true) 
  {
    Serial.printf("Device::otaUpdating %d\n", Device::otaUpdating);
    if (ota.updateStart()) 
    {
      if (http.notifyOTAUpdate(Device::macAddress)) 
      {
        Serial.println("Device update reported successfully");
        http.updateDeviceStatus(Device::macAddress, OTA_SUCCESS);
        ESP.restart();
      } 
      else 
      {
        Serial.println("Failed to report update to server");
        http.updateDeviceStatus(Device::macAddress, OTA_ROLL_BACK);
      }
      break;
    }
    if(!ota.updated) 
      break;
 
  }
  Device::otaUpdating = false;
  Serial.printf("Device::otaUpdating %d\n", Device::otaUpdating);
  Serial.println("OTA Task Deleted");
  vTaskDelete(NULL);
}
