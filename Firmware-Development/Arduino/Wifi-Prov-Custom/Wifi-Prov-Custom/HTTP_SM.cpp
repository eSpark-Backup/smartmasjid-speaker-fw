#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "HTTP_SM.hpp"
const char* apiUrl = "http://smartmasjid-api-qa.futuriklabs.com/external/adhan/start";
const int masjidId = 1;
WiFiClient client;

bool buttonPressed = false;
void handleButtonInterrupt()
{
    buttonPressed = true;
}

void sendPostRequest()
{
  StaticJsonDocument<200> doc;
    doc["masjidId"] = 3;
  String jsonString;
  serializeJson(doc, jsonString);

  // Send the POST request
  HTTPClient http;
  http.begin(client, apiUrl);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(jsonString);

  // Print the response
  Serial.print("HTTP Response Code: ");
  Serial.println(httpResponseCode);
  Serial.println(http.getString());

  // End the HTTP connection
  http.end();
}
