#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "SM_Http.hpp"
#include "SM_Json.hpp"
#include "Device.hpp"
#include "BLE.hpp"
#include "Mqtt.hpp"

WiFiClientSecure *client = new WiFiClientSecure;
// WiFiClientSecure client;

bool SmHttp::startAdhan()
{
  // if (WiFi.status() != WL_CONNECTED)
  //   return false;

  // // client->setCACert(ca_cert);
  // // client->setInsecure();
  // // Send the POST request
  // HTTPClient https;
  // // std::string concatenatedString = smHost;
  // // concatenatedString += adhanStartEndpoint;
  // https.begin(*client,"https://smartmasjid-api-qa.futuriklabs.com/external/adhan/start");
  // https.addHeader("Content-Type", "application/json");
  // https.addHeader("x-api-key", EXTERNAL_API_KEY);

  // char masjidIdJson[64]; 
  // SM_Json::create(masjidIdJson, sizeof(masjidIdJson),
  //                     std::make_pair("masjidId", MASJID_ID_TEST));
  // Serial.println(masjidIdJson);
  
  // int httpCode = https.POST(masjidIdJson);
  // const char* response = https.getString().c_str();

  // https.end();

  // if ((httpCode == HTTP_STATUS_OK) || (httpCode == HTTP_STATUS_CREATED)) 
  // {
  //   Serial.println("Adhan Started!");
  //   Serial.println(response);
  //   return true;
  // } 
  // else 
  // {
  //   Serial.print("Failed to start adhan with error code: ");
  //   Serial.println(httpCode);
  //   Serial.println(response);
  //   return false;
  // }
  return true;
}

bool SmHttp::sendDeviceTelemetry(const char* macAddress, uint8_t retryInterval, uint16_t maxRetryDuration) 
{
  if (WiFi.status() != WL_CONNECTED)
    return false;

  unsigned long startTime = millis();
  while ((millis() - startTime) < TO_MS(maxRetryDuration)) 
  {
    if (sendDeviceTelemetry(macAddress)) 
    {
      return true;
    }
    Serial.println("Failed to send telemetry, retrying in 10 seconds...");
    delay(TO_MS(retryInterval));
  }
  return false;
}

bool SmHttp::sendDeviceTelemetry(const char* macAddress) 
{
  if (WiFi.status() != WL_CONNECTED)
    return false;

  client->setInsecure();

  // Make HTTP POST request
  HTTPClient https;
  Serial.print("[HTTPS] begin...\n");
  std::string concatenatedString = smHost;
  concatenatedString += telemetryEndpoint;
  // https.begin(*client,"https://smartmasjid-api-qa.futuriklabs.com/external/device/telemetry");
  https.begin(*client,concatenatedString.c_str());
  Serial.print("[HTTPS] POST...\n");
  https.addHeader("x-api-key", EXTERNAL_API_KEY);
  https.addHeader("Content-Type", "application/json");
  
  char telemetryJson[64]; 
  SM_Json::create(telemetryJson, sizeof(telemetryJson),
                      std::make_pair("battery", BATTERY_HEALTH),
                      std::make_pair("macAddress", macAddress));
  Serial.println(telemetryJson);
  
  Serial.print("check point.....................1\n");
  int httpCode = https.POST(telemetryJson);
  Serial.print("check point.....................2\n");
  const char* response = https.getString().c_str();
  Serial.print("check point.....................3\n");
  https.end();

  if ((httpCode == HTTP_STATUS_OK) || (httpCode == HTTP_STATUS_CREATED)) 
  {
    Serial.println("Telemetry sent successfully!");
    Serial.println(response);
    retry = 1;
    return true;
  } 
  else 
  {
    Serial.print("Failed to send telemetry with error code: ");
    Serial.println(httpCode);

    // if(retry>6)
    // {
    //   if(httpCode == DEVCICE_NOT_FOUND)
    //       { 
    //         Serial.println("Device not found");  
    //        //Reset
    //        MQTTClient mqtt;
    //        mqtt.unsubscribeTopics();
    //        Device::deleteMasjidId();
    //        Device::deleteWificredentials();
    //        Serial.println("Device is Reset ...");  
    //        retry = 1;
    //        return false;
    //        }
    //   else
    //      {  
    //        //reset esp32
    //        Serial.println("Restart ESP.....");
    //        ESP.restart();
    //      }     

    // }
    // retry++;
    Serial.println(response);
    return false;
  }
}

bool SmHttp::stopAdhan()
{
//  if (WiFi.status() != WL_CONNECTED)
//     return false;
//   // client->setCACert(ca_cert);
//    client.setInsecure();
//   // Send the POST request
//   char concatenated[256]; 
//   strcpy(concatenated, smHost);
//   strcat(concatenated, adhanStopEndpoint);
//   HTTPClient https;
//   https.begin(client,concatenated);
//   https.addHeader("Content-Type", "application/json");
//   https.addHeader("x-api-key", EXTERNAL_API_KEY);

//   char masjidIdJson[64]; 
//   SM_Json::create(masjidIdJson, sizeof(masjidIdJson),
//                       std::make_pair("masjidId", MASJID_ID_TEST));
//   Serial.println(masjidIdJson);
  
//   int httpCode = https.POST(masjidIdJson);
//   const char* response = https.getString().c_str();

//   https.end();

//   if ((httpCode == HTTP_STATUS_OK) || (httpCode == HTTP_STATUS_CREATED)) 
//   {
//     Serial.println("Adhan Stopped!");
//     Serial.println(response);
//     return true;
//   } 
//   else 
//   {
//     Serial.print("Failed to stop adhan with error code: ");
//     Serial.println(httpCode);
//     Serial.println(response);
//     return false;
//   }
return true;
}
