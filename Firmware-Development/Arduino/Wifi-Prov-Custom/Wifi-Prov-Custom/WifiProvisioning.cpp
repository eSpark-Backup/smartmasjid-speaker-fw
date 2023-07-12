#include "WifiProvisioning.hpp"
#include "Device.h"

bool WifiProvisioning::connectWifi(const char *ssid, const char *pass,int retries)
{ 
  if (ssid != NULL && pass != NULL)
  {
    // Serial.print(count);

    // try to connect to WiFi
    Serial.printf("Connecting to %s...\n", ssid);
    WiFi.onEvent(std::bind(&WifiProvisioning::wifiGotIP, this, std::placeholders::_1, std::placeholders::_2), WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED && retries-- > 0)
    {
      delay(1000);
      Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    else
    {
      Serial.println("Failed to connect to WiFi");
    }
  }
  return false;
}


bool WifiProvisioning::connectWifiFromNVS(void)
{
  preferences.begin("credentials", false);
  String ssid = preferences.getString("ssid", "");
  String pass = preferences.getString("pass", "");

  if (ssid == "" || pass == "")
  {
    Serial.println("No values saved for ssid or password");
  }
  else if (connectWifi(ssid.c_str(), pass.c_str()))
  {
    return true;
  }
  return false;
}

void WifiProvisioning::wifiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {

  // Print network details
  Serial.print("Connected to Wi-Fi network with IP address: ");
  Serial.println(WiFi.localIP());

  if (sendDeviceTelemetry(MAC_ADDRESS, RETRY_INTERVAL, MAX_RETRY_DURATION))
  {
    deviceTelemetryStatus = TELEMETRY_SENT;
  }
}
bool WifiProvisioning::sendDeviceTelemetry(const char* macAddress, uint8_t retryInterval, uint16_t maxRetryDuration) 
{
  // Start retry loop
  unsigned long startTime = millis();
  while (millis() - startTime < (maxRetryDuration * SECONDS)) 
  {
    if (sendDeviceTelemetry(macAddress)) 
    {
      return true;
    }
    Serial.println("Failed to send telemetry, retrying in 10 seconds...");
    delay(retryInterval * SECONDS);
  }
  return false;
}
bool WifiProvisioning::sendDeviceTelemetry(const char* macAddress) {

  char apiEndpoint[100];
  snprintf(apiEndpoint, sizeof(apiEndpoint), apiEndpointTemplate);
  
  // Make HTTP POST request
  HTTPClient http;
  http.begin(apiHost, 80, apiEndpoint);
  http.addHeader("x-api-key", "b8z6X5fzbjNDGHz15XtPVzEkO3h5mCT7q4Ei8AupX9DD12PxL94rhnZHarHc8O2H");
  http.addHeader("Content-Type", "application/json");
  char body[64];
  createTelemetryJson(body, sizeof(body), macAddress, 90);
  
  int httpCode = http.POST(body);
  const char* response = http.getString().c_str();
  http.end();

  // Check for successful response
  if ((httpCode == HTTP_STATUS_OK) || (httpCode == HTTP_STATUS_CREATED)) 
  {
    Serial.println("Telemetry sent successfully!");
    Serial.println(response);
    return true;
  } 
  else 
  {
    Serial.print("Failed to send telemetry with error code: ");
    Serial.println(httpCode);
    Serial.println(response);
    return false;
  }
}

bool WifiProvisioning::storeWifiCredsRAM(const char* ssid, const char* pass)
{
  strcpy(gSsid, ssid);
  strcpy(gPass, pass);
  return true;
}

bool WifiProvisioning::storeWifiCredsNVS(void)
{
  storeWifiCredsNVS(gSsid, gPass);
  return true; 
}

bool WifiProvisioning::storeWifiCredsNVS(const char* ssid, const char* pass)
{
  preferences.putString("ssid", ssid);
  preferences.putString("pass", pass);
  return true;
}
bool WifiProvisioning::removeWifiCredsNVS(void)
{
  removeWifiCredsNVS(gSsid, gPass);
  return true; 
}
bool WifiProvisioning::removeWifiCredsNVS(const char* ssid, const char* pass)
{
  preferences.remove("ssid");
  preferences.remove("pass");
  return true;
}
void WifiProvisioning::createTelemetryJson(char* json, uint8_t size, const char* macAddress, int battery) {
  // Create a JSON document
  StaticJsonDocument<64> doc;

  // Add values to the document
  doc["battery"] = battery;
  doc["macAddress"] =macAddress;
  

  // Serialize the JSON document
  serializeJson(doc, json, size);

  // Print the JSON string
  Serial.println(json);
}

bool WifiProvisioning::isTelemetrySent(void)
{
  return (deviceTelemetryStatus==TELEMETRY_SENT);
}
