#include <WiFi.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include <Preferences.h>

#define WIFI_RETRY 3  // number of times to retry WiFi connection

BluetoothSerial SerialBT;
Preferences preferences;

String ssid;
String password;

const char* tempssid = "";
const char* temppassword = "";

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32");  // set Bluetooth name

  preferences.begin("credentials", false);
  ssid = preferences.getString("ssid", ""); 
  password = preferences.getString("password", "");

  if (ssid == "" || password == ""){
    Serial.println("No values saved for ssid or password");
  }
  
  else if (ssid != NULL && password != NULL) {
    // try to connect to WiFi
    Serial.printf("Connecting to %s...\n", ssid);
    WiFi.begin(ssid.c_str(), password.c_str());
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

    tempssid = doc["ssid"];
    temppassword = doc["password"];
    Serial.println(tempssid);
    Serial.println(temppassword);
    preferences.putString("ssid", tempssid);
    preferences.putString("password", temppassword);

    // switch to WiFi mode and connect
    Serial.printf("Connecting to %s...\n", ssid);
    WiFi.begin(tempssid, temppassword);
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
