#pragma once
#include"Arduino.h"
// This sketch provide the functionality of OTA Firmware Upgrade
#include "WiFi.h"
#include "HttpsOTAUpdate.h"



// static const char *url = "https://192.168.18.160:4440/firmware.bin";  //state url of your firmware image

static const char *server_certificate = "-----BEGIN CERTIFICATE-----\n" 
"MIIDgTCCAmmgAwIBAgIUaLwy8V7VAObHGSZYv6T3O7eK3eMwDQYJKoZIhvcNAQEL\n" 
"BQAwaTELMAkGA1UEBhMCUEsxDjAMBgNVBAgMBVNpbmRoMRAwDgYDVQQHDAdLYXJh\n" 
"Y2hpMQswCQYDVQQKDAJFRTELMAkGA1UECwwCU1MxCzAJBgNVBAMMAmRkMREwDwYJ\n" 
"KoZIhvcNAQkBFgJmZjAeFw0yNDA2MTAxNTE1MThaFw0yNTA2MTAxNTE1MThaMGkx\n" 
"CzAJBgNVBAYTAlBLMQ4wDAYDVQQIDAVTaW5kaDEQMA4GA1UEBwwHS2FyYWNoaTEL\n" 
"MAkGA1UECgwCRUUxCzAJBgNVBAsMAlNTMQswCQYDVQQDDAJkZDERMA8GCSqGSIb3\n" 
"DQEJARYCZmYwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC1hdrTggzm\n" 
"YXfQJVoi7Sf02twkayHJgyx+EreeNr3HmKzPijC7Xara2ngxDsFc421ePcQ3unwg\n" 
"1WVbD7ipFsUTV8yXe9FPlw3GIOIPPzz7pdoHKMgLgWhkxf7/88jk1Vx+B/panKAa\n" 
"lm32okugJNTzyCGU5wAcoKRCIlQ8M7/vHaUqHCXnjhJwyRo2G+49uqqHMsKs+HLG\n" 
"mwZWreLTRh2W8WdR0feP0OlKhF4nLItwlYAFd0M+ajPU83PbVxvRgcr+fc3/IEKL\n" 
"0/wo+rKuwXHxH4C2DkDhdiOzF94a68b1bIK4FwAJSz+u5743mk+MD1mEPK1BD0jk\n" 
"bUUQIXdRl8vXAgMBAAGjITAfMB0GA1UdDgQWBBS/rI/YzBcjBQD6zvRSxa9IAfXp\n" 
"RTANBgkqhkiG9w0BAQsFAAOCAQEAOZHC+j4DtK5BqNPVWepJjynPxKEU0CYzvPHj\n" 
"KyuMKV6DsoVfVWu7k8MeUZ81b+tcS30pncWlSexgO3hNRuLCRo4UqFziYQRHE7CB\n" 
"S70tEYxK+JI3nkjNYURwe8Z2KmqVkDdf2N6QOciNY1IlJFG3O0TzcHT8ITpLksKv\n" 
"j694adWUy38q5LP7T3FlRllp2VXHjjmi920ZFIYb0au+KDku209yi1o+UT/2VqEo\n" 
"fj3ECLsGslEHwcfYcarMW5L42sWGEp4SXKnROLyamyS86aGC2l+M/zge+5Nbl/ve\n" 
"C3FNA5pDagDlvuRWfeu5tvT0FPYp6lPf5fbxzIkI5KpAJoSqtg==\n" 
"-----END CERTIFICATE-----\n";
static HttpsOTAStatus_t otastatus;
void awsOtaTask(void* pvParameters);



class AWS_OTA
{ 
   
  public:
   const char* URL;
   bool OTA_UPDATED = false;
   static void HttpEvent(HttpEvent_t *event);
   void setUp(const char* url);
   bool UpdateStart();
   void createTask(const char* url);
};

extern AWS_OTA ota;
