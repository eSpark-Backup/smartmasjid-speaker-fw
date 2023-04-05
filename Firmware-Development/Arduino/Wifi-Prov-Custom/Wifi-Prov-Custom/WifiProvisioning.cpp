#include "WifiProvisioning.hpp"

// Replace with your API details
const char* api_host = "smartmasjid-api-qa.futuriklabs.com";
const char* api_endpoint_template = "/devices/%s/telemetry";
const char* macAddress = "abc:123:efg:456";
bool deviceTelmetryStatus= false;

static char gssid[32];
static char gpass[32];

Preferences preferences;
bool connectWifi(const char *ssid, const char *pass)
{
  if (ssid != NULL && pass != NULL)
  {
    // try to connect to WiFi
    Serial.printf("Connecting to %s...\n", ssid);
    WiFi.onEvent(wifiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.begin(ssid, pass);
    int retries = WIFI_RETRY;
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

bool connectWifiFromNVS(void)
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

void wifiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {

  // Print network details
  Serial.print("Connected to Wi-Fi network with IP address: ");
  Serial.println(WiFi.localIP());

  if (sendDeviceTelemetry(macAddress, RETRY_INTERVAL, MAX_RETRY_DURATION))
  {
    deviceTelmetryStatus = true;
  }
}
bool sendDeviceTelemetry(const char* mac_address, uint8_t retry_interval, uint16_t max_retry_duration) 
{
  // Start retry loop
  unsigned long start_time = millis();
  while (millis() - start_time < (max_retry_duration * SECONDS)) 
  {
    if (sendDeviceTelemetry(mac_address)) 
    {
      return true;
    }
    Serial.println("Failed to send telemetry, retrying in 10 seconds...");
    delay(retry_interval * SECONDS);
  }
  return false;
}
bool sendDeviceTelemetry(const char* mac_address) {
  
  // Construct API endpoint with MAC address
  char api_endpoint[100];
  snprintf(api_endpoint, sizeof(api_endpoint), api_endpoint_template, mac_address);
  
  // Make HTTP POST request
  HTTPClient http;
  http.begin(api_host, 80, api_endpoint);
  http.addHeader("Content-Type", "application/json");
  
  char body[64];
  createTelemetryJson(body, sizeof(body), "ONLINE", 70);
  
  int http_code = http.POST(body);
  const char* response = http.getString().c_str();
  http.end();

  // Check for successful response
  if ((http_code == HTTP_STATUS_OK) || (http_code == HTTP_STATUS_CREATED)) 
  {
    Serial.println("Telemetry sent successfully!");
    Serial.println(response);
    return true;
  } 
  else 
  {
    Serial.print("Failed to send telemetry with error code: ");
    Serial.println(http_code);
    Serial.println(response);
    return false;
  }
}

bool storeWifiCredsRAM(const char* ssid, const char* pass)
{
  strcpy(gssid, ssid);
  strcpy(gpass, pass);
  return true;
}

bool storeWifiCredsNVS(void)
{
  storeWifiCredsNVS(gssid, gpass);
  return true; 
}

bool storeWifiCredsNVS(const char* ssid, const char* pass)
{
  preferences.putString("ssid", ssid);
  preferences.putString("pass", pass);
  return true;
}

void createTelemetryJson(char* json, uint8_t size, const char* status, int battery) {
  // Create a JSON document
  StaticJsonDocument<64> doc;

  // Add values to the document
  doc["status"] = status;
  doc["battery"] = battery;

  // Serialize the JSON document
  serializeJson(doc, json, size);

  // Print the JSON string
  Serial.println(json);
}
