#include <WiFi.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include <Preferences.h>

#define WIFI_RETRY 3  // number of times to retry WiFi connection

BluetoothSerial SerialBT;
Preferences preferences;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32");  // set Bluetooth name

  preferences.begin("wifi_credentials", false);  // open NVS flash memory

  if (preferences.getBytesLength("ssid") > 0 && preferences.getBytesLength("pass") > 0) {
    // read stored credentials
    char ssid[32], pass[64];
    preferences.getBytes("ssid", ssid, 32);
    preferences.getBytes("pass", pass, 64);

    // try to connect to WiFi
    Serial.printf("Connecting to %s...\n", ssid);
    WiFi.begin(ssid, pass);
    int retries = WIFI_RETRY;
    while (WiFi.status() != WL_CONNECTED && retries-- > 0) {
      delay(1000);
      Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected to WiFi");
      return;
    }
  }

  // switch to Bluetooth mode
  Serial.println("Switching to Bluetooth mode");
  WiFi.mode(WIFI_OFF);
  SerialBT.begin("ESP32");
  Serial.println("Waiting for Bluetooth connection");
}

void loop() {
  if (SerialBT.available()) {
    // read JSON data from Bluetooth
    StaticJsonDocument<128> doc;
    DeserializationError error = deserializeJson(doc, SerialBT.readString());
    if (error) {
      Serial.println("Error parsing JSON");
      return;
    }

    // extract SSID and password from JSON
    const char* ssid = doc["ssid"];
    const char* pass = doc["pass"];

    // store credentials in NVS flash
    preferences.putBytes("ssid", (const uint8_t*)ssid, strlen(ssid) + 1);
    preferences.putBytes("pass", (const uint8_t*)pass, strlen(pass) + 1);

    // switch to WiFi mode and connect
    Serial.printf("Connecting to %s...\n", ssid);
    WiFi.begin(ssid, pass);
    int retries = WIFI_RETRY;
    while (WiFi.status() != WL_CONNECTED && retries-- > 0) {
      delay(1000);
      Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected to WiFi");
      return;
    }
    else {
      Serial.println("Failed to connect to WiFi");
    }
  }
}
