#pragma once

#include <WiFiClientSecure.h>
#include "Mqtt.hpp"
#define HTTP_STATUS_OK 200
#define HTTP_STATUS_CREATED 201
#define DEVCICE_NOT_FOUND -766
#define EXTERNAL_API_KEY "b8z6X5fzbjNDGHz15XtPVzEkO3h5mCT7q4Ei8AupX9DD12PxL94rhnZHarHc8O2H"
#define TO_MS(seconds) (seconds * 1000)


class SmHttp 
{
  private:
    const char* smHost = "https://smartmasjid-api-qa.futuriklabs.com";
    const char* telemetryEndpoint = "/external/device/telemetry";
    const char* adhanStartEndpoint = "/external/adhan/start";
    const char* adhanStopEndpoint = "/external/adhan/stop";
    int retry =  1;
    
//     const char* ca_cert = \
// "-----BEGIN CERTIFICATE-----\n"\
// "MIIEXjCCA0agAwIBAgITB3MSOAudZoijOx7Zv5zNpo4ODzANBgkqhkiG9w0BAQsF\n"\
// "ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n"\
// "b24gUm9vdCBDQSAxMB4XDTIyMDgyMzIyMjEyOFoXDTMwMDgyMzIyMjEyOFowPDEL\n"\
// "MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEcMBoGA1UEAxMTQW1hem9uIFJT\n"\
// "QSAyMDQ4IE0wMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOtxLKnL\n"\
// "H4gokjIwr4pXD3i3NyWVVYesZ1yX0yLI2qIUZ2t88Gfa4gMqs1YSXca1R/lnCKeT\n"\
// "epWSGA+0+fkQNpp/L4C2T7oTTsddUx7g3ZYzByDTlrwS5HRQQqEFE3O1T5tEJP4t\n"\
// "f+28IoXsNiEzl3UGzicYgtzj2cWCB41eJgEmJmcf2T8TzzK6a614ZPyq/w4CPAff\n"\
// "nAV4coz96nW3AyiE2uhuB4zQUIXvgVSycW7sbWLvj5TDXunEpNCRwC4kkZjK7rol\n"\
// "jtT2cbb7W2s4Bkg3R42G3PLqBvt2N32e/0JOTViCk8/iccJ4sXqrS1uUN4iB5Nmv\n"\
// "JK74csVl+0u0UecCAwEAAaOCAVowggFWMBIGA1UdEwEB/wQIMAYBAf8CAQAwDgYD\n"\
// "VR0PAQH/BAQDAgGGMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjAdBgNV\n"\
// "HQ4EFgQUgbgOY4qJEhjl+js7UJWf5uWQE4UwHwYDVR0jBBgwFoAUhBjMhTTsvAyU\n"\
// "lC4IWZzHshBOCggwewYIKwYBBQUHAQEEbzBtMC8GCCsGAQUFBzABhiNodHRwOi8v\n"\
// "b2NzcC5yb290Y2ExLmFtYXpvbnRydXN0LmNvbTA6BggrBgEFBQcwAoYuaHR0cDov\n"\
// "L2NydC5yb290Y2ExLmFtYXpvbnRydXN0LmNvbS9yb290Y2ExLmNlcjA/BgNVHR8E\n"\
// "ODA2MDSgMqAwhi5odHRwOi8vY3JsLnJvb3RjYTEuYW1hem9udHJ1c3QuY29tL3Jv\n"\
// "b3RjYTEuY3JsMBMGA1UdIAQMMAowCAYGZ4EMAQIBMA0GCSqGSIb3DQEBCwUAA4IB\n"\
// "AQCtAN4CBSMuBjJitGuxlBbkEUDeK/pZwTXv4KqPK0G50fOHOQAd8j21p0cMBgbG\n"\
// "kfMHVwLU7b0XwZCav0h1ogdPMN1KakK1DT0VwA/+hFvGPJnMV1Kx2G4S1ZaSk0uU\n"\
// "5QfoiYIIano01J5k4T2HapKQmmOhS/iPtuo00wW+IMLeBuKMn3OLn005hcrOGTad\n"\
// "hcmeyfhQP7Z+iKHvyoQGi1C0ClymHETx/chhQGDyYSWqB/THwnN15AwLQo0E5V9E\n"\
// "SJlbe4mBlqeInUsNYugExNf+tOiybcrswBy8OFsd34XOW3rjSUtsuafd9AWySa3h\n"\
// "xRRrwszrzX/WWGm6wyB+f7C4\n"\
// "-----END CERTIFICATE-----\n";

  public:
    bool startAdhan();  
    bool sendDeviceTelemetry(const char* macAddress, uint8_t retryInterval, uint16_t maxRetryDuration);
    bool sendDeviceTelemetry(const char* macAddress);
    bool stopAdhan(); 
    bool sendHttpsPostRequest(const char* url,const char* json);
};
