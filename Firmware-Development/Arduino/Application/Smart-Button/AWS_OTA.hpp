#pragma once
#include"Arduino.h"
// This sketch provide the functionality of OTA Firmware Upgrade
#include "WiFi.h"
#include "HttpsOTAUpdate.h"

static const char *url = "https://192.168.18.160:4442/firmware.bin";  //state url of your firmware image

static const char *server_certificate = "-----BEGIN CERTIFICATE-----\n"
"MIIDqzCCApOgAwIBAgIUJztRzu+i8mnamBOdGEd7xWS/RtMwDQYJKoZIhvcNAQEL\n"
"BQAwfjELMAkGA1UEBhMCUEsxDjAMBgNVBAgMBVNpbmRoMRAwDgYDVQQHDAdLYXJh\n"
"Y2hpMQ8wDQYDVQQKDAZlU3BhcmsxDTALBgNVBAsMBHBhcmsxDDAKBgNVBAMMA1VV\n"
"ZTEfMB0GCSqGSIb3DQEJARYQZVNwYXJrQGdtYWlsLmNvbTAeFw0yNDA2MDMxNjI2\n"
"MzdaFw0yNTA2MDMxNjI2MzdaMH4xCzAJBgNVBAYTAlBLMQ4wDAYDVQQIDAVTaW5k\n"
"aDEQMA4GA1UEBwwHS2FyYWNoaTEPMA0GA1UECgwGZVNwYXJrMQ0wCwYDVQQLDARw\n"
"YXJrMQwwCgYDVQQDDANVVWUxHzAdBgkqhkiG9w0BCQEWEGVTcGFya0BnbWFpbC5j\n"
"b20wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCZpKH4Wrs9f+vks5Qy\n"
"neFrNxz7rkl+Nl44iXaj7wOHoOCIO3vVUHlLNP/cPFIZR8tqkgRiI0pKWfeCRCIF\n"
"zH0S/jWqLiG1zwDmzY2GRqAvdw4hel/SyeXYPiz6ytlExlTdd9qgaE+jSOIEHuZL\n"
"FWBXKOh6hNpKe/0LdDFa2F+yj6L23Wa2llqk6yxBNmHYyfa5x/PiWrBv7af1ufCV\n"
"SM6sQVrnmuuVa8QQv4MAuwFZDeFeccMzJfpEXyz7imIgoaTGl2hZc4Aar4F9/Wjy\n"
"hM29QuGJLyteQHZdxb7Rm+FLryh4KwOb2cDIIJ0XlGMjKyToMNBdqwE0FjtZITm9\n"
"a1efAgMBAAGjITAfMB0GA1UdDgQWBBROgTL5s/ZA4fCTzGg39DYxuhkBFDANBgkq\n"
"hkiG9w0BAQsFAAOCAQEAD9Xwae8iY/bIX4ot38pybG/4ZlBM69wukRKI7mveXdjT\n"
"G9SgfxgqbWpuW1tVm7nyQ42fdt3PaefiwA2QiPsNM/u/LJgm/rfRIQG2mjsL7ecP\n"
"6OQ5m7G0/FdiNtfwpzfv3hDp3QP2sX7Nt6avJtlDWSDBWYCJn12INV7FKOMFBU/i\n"
"fZdNOUnHGgRKTq0MFE4G4mJb8KB3IXJmTK0SwKmcJdNmxewOk46zLw7yYYMnbfLA\n"
"77MMvGvbOchn5+CoM66xsZFIY17i6IB2pGE++nUVOcjEksKodSFzg8bEMj49ebvF\n"
"rl22i8klEuJf2GxCfyRGttCHnpyapoaS++GuBb0DLQ==\n"
"-----END CERTIFICATE-----\n";

static HttpsOTAStatus_t otastatus;




class AWS_OTA
{
  public:
    static void HttpEvent(HttpEvent_t *event);
    void setUp(const char* url);
    bool UpdateStart();
};