#pragma once
#include"Arduino.h"
// This sketch provide the functionality of OTA Firmware Upgrade
#include "WiFi.h"
#include "HttpsOTAUpdate.h"



// static const char *url = "https://192.168.18.160:4440/firmware.bin";  //state url of your firmware image

static const char *server_certificate = 
"-----BEGIN CERTIFICATE-----\n"
"MIIDZTCCAk2gAwIBAgIUFNNJsqwI/CLYCz1Y2puJKjTsOscwDQYJKoZIhvcNAQEL\n"
"BQAwWzELMAkGA1UEBhMCUEsxCjAIBgNVBAgMAWExCjAIBgNVBAcMAWIxCjAIBgNV\n"
"BAoMAWMxCjAIBgNVBAsMAWQxCjAIBgNVBAMMAWUxEDAOBgkqhkiG9w0BCQEWAWYw\n"
"HhcNMjQwOTAyMDg1MTU3WhcNMjUwOTAyMDg1MTU3WjBbMQswCQYDVQQGEwJQSzEK\n"
"MAgGA1UECAwBYTEKMAgGA1UEBwwBYjEKMAgGA1UECgwBYzEKMAgGA1UECwwBZDEK\n"
"MAgGA1UEAwwBZTEQMA4GCSqGSIb3DQEJARYBZjCCASIwDQYJKoZIhvcNAQEBBQAD\n"
"ggEPADCCAQoCggEBAKvgJq2QlbY4GvVtVfgc73cNf2X/Ed2zZsgkUDgD8ZGaoGcH\n"
"w/YkQ25VjS43kDVKx93bzQKXfN2WglPZ9TF/yKXikNesXVS+s8fpe+Mvepqxm3e0\n"
"PBz1yL52D8di9roX3KDlOzax1YbhRaO7fu42lKqaE+9siplsRDkGDhR1Tpk3kr4H\n"
"q+mRxF7zy57yrd+gGUMTRQudosJSBIUKTVXCaVsAxmn6e9pS+cznm4o2BSWT8nIy\n"
"DxjDIr4HytoyU8x0N9D3c02YBOSJiGuyl+H6xI7kjqXcbWqELh0yu/TWicEaD7te\n"
"5/7GlI+LJdmqNQZaO8cu3cD/n5DV+KWTW8C8nicCAwEAAaMhMB8wHQYDVR0OBBYE\n"
"FEvJoPC8T1eDodR46Tx7SMrwdJLNMA0GCSqGSIb3DQEBCwUAA4IBAQA6a7ByUFOL\n"
"i5ihjYja5A3Nmb+Iu4OaIKX8DttrPRPWJOG8CRU7wa1bQOo7vqeSjaC3VxSfKh4V\n"
"4gvrzqnB98ZP+vOp6D7vk7BGwPsgrkKOZ6ltHZW9UDzTYIRRf1ykr3aDIEO/gena\n"
"/NPSy4yJX1G2tTVUT6Xbusm/5FRspcPJ8rc40c/GggflyMByVOvbXylQx3OOhKK1\n"
"x7G+9KSJJeBuOEHRA0V8xcJBRnVT7qlf0mxqz+zx1X88nkSUCcIIUM1NQGOKN7nn\n"
"5yXcgn6GluF/z5innsl+psyWHvV2fLrK8gA+eFpXSvTAq1qY6mKlZPS//YxOlpBO\n"
"idjmWfi8qYij\n"
"-----END CERTIFICATE-----\n";
static HttpsOTAStatus_t otastatus;
void awsOtaTask(void* pvParameters);

class AWS_OTA
{ 
  public:
   const char* URL;
   bool OTA_UPDATED = false;
   static void httpEventHandler(HttpEvent_t *event);
   void setUp(const char* url);
   bool UpdateStart();
   void createTask(const char* url);
};

extern AWS_OTA ota;
