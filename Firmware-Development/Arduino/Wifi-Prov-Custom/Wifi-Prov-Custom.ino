#include <WiFi.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include <Preferences.h>

#define WIFI_RETRY 3  // number of times to retry WiFi connection

BluetoothSerial SerialBT;
Preferences preferences;

bool connectWifi(const char* ssid, const char* password)
{
  if (ssid != NULL && password != NULL) 
  {
    // try to connect to WiFi
    Serial.printf("Connecting to %s...\n", ssid);
    WiFi.begin(ssid, password);
    int retries = WIFI_RETRY;
    while (WiFi.status() != WL_CONNECTED && retries-- > 0) 
    {
      delay(1000);
      Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) 
    {
      Serial.println("Connected to WiFi");
      return true;
    }
    else 
    {
      Serial.println("Failed to connect to WiFi");
    }
  }
  return false;
}

bool readCredsOverBLE(void)
{
  if (SerialBT.available()) {
    // read JSON data from Bluetooth
    StaticJsonDocument<128> doc;
    DeserializationError error = deserializeJson(doc, SerialBT.readString());
    if (error) 
    {
      Serial.println("Error parsing JSON");
      return false;
    }
    
    const char* ssid = doc["ssid"];
    const char* password = doc["password"];

    preferences.putString("ssid", ssid);
    preferences.putString("password", password);

    // switch to WiFi mode and connect
    if (connectWifi(ssid, password))
    {
      SerialBT.end();
      return true;
    }
    SerialBT.println("Invalid ssid or password"); 
  }
  return false;
}

bool connectWifiFromNVS(void)
{
  preferences.begin("credentials", false);
  String ssid = preferences.getString("ssid", ""); 
  String password = preferences.getString("password", "");

  if (ssid == "" || password == "")
  {
    Serial.println("No values saved for ssid or password");
  }
  else if (connectWifi(ssid.c_str(),password.c_str()))
  {
    return true;
  }
  return false;
}

void setup() {
  Serial.begin(115200);

  if(connectWifiFromNVS())
  {
    return;
  }
  
  // switch to Bluetooth mode
  Serial.println("Switching to Bluetooth mode");
  WiFi.mode(WIFI_OFF);
  SerialBT.begin("ESP32");
  Serial.println("Waiting for Bluetooth connection");
  while (!readCredsOverBLE());
}

void loop() {
  if (WiFi.status() != WL_CONNECTED)
  {
    connectWifiFromNVS();
  }
}
