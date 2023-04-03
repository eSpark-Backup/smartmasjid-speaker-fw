#include "WifiProvisioning.hpp"

Preferences preferences;
bool connectWifi(const char *ssid, const char *password)
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

bool connectWifiFromNVS(void)
{
  preferences.begin("credentials", false);
  String ssid = preferences.getString("ssid", "");
  String password = preferences.getString("password", "");

  if (ssid == "" || password == "")
  {
    Serial.println("No values saved for ssid or password");
  }
  else if (connectWifi(ssid.c_str(), password.c_str()))
  {
    return true;
  }
  return false;
}
