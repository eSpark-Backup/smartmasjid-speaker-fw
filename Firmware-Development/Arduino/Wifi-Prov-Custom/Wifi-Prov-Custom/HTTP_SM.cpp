#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "HTTP_SM.hpp"
#include <ArduinoJson.h>
const char* apiUrl = "http://smartmasjid-api-qa.futuriklabs.com/external/adhan/start";

void Smhttp::AdhanstartRequest()
{
  StaticJsonDocument<200> doc;
  doc["masjidId"] = 3;
  String jsonString;
  serializeJson(doc, jsonString);

  // Send the POST request
  HTTPClient http;
  http.begin(apiUrl);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("x-api-key", EXTERNAL_API_KEY);
  int httpResponseCode = http.POST(jsonString);

  // Print the response
  Serial.print("HTTP Response Code: ");
  Serial.println(httpResponseCode);
  Serial.println(http.getString());

  // End the HTTP connection
  http.end();
}
