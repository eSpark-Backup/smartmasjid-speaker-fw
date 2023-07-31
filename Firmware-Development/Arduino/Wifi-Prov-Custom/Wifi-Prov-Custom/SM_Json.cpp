#include <ArduinoJson.h>
#include <WiFi.h>
#include "SM_Json.hpp"
#include "Device.h"

void SM_Json::createFactoryInfoJson(char* json, uint8_t size) 
{
  char deviceMacAddress[20];
  getDeviceMAC(deviceMacAddress);
  // Create a JSON document
  StaticJsonDocument<256> doc;

  // Set the values in the JSON document
  doc["device_model"] = SM_MODEL;
  doc["mac_address"] = deviceMacAddress;
  doc["firmware_rev"] = SM_FIRMWARE_VER;

  // Serialize the JSON document to a string
  
  // serializeJson(doc, jsonString);
  serializeJson(doc, json, size);
}

void SM_Json::createTelemetryJson(const char* macAddress, int battery, char* json, uint8_t size) {
  // Create a JSON document
  StaticJsonDocument<64> doc;

  // Add values to the document
  doc["battery"] = battery;
  doc["macAddress"] = macAddress;

  // Serialize the JSON document
  serializeJson(doc, json, size);

  // Print the JSON string
  Serial.println(json);
}
