#include "BLE.hpp"
#include "Device.h"
WifiProvisioning* pWifiProv = NULL;

BLEServer *pServer = NULL;
BLEService *pService = NULL;
BLECharacteristic *pTxCharacteristic;
BLECharacteristic *pDeviceCharacteristic;
BLECharacteristic *pProvCharacteristic;
String jsonPacket;
// String printDeviceAddress();
String  jsonPacket1 = "{\"device_model\": \"SBB23V1\", \"mac_address\": \"";
String  jsonPacket2;
String jsonPacket3 = "\", \"firmware_rev\": \"1.0\"}";
const char *ssid;
const char *pass;

class MyServerCallbacks : public BLEServerCallbacks
{

  void onConnect(BLEServer *pServer)
  {
    Serial.println("BLE : device connected");
    Serial.println("BLE MAC Address:");
    initBluetooth();
    Serial.println(" ");
    const uint8_t* point = esp_bt_dev_get_address();
    char mac[18];
    sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X", (int)point[0], (int)point[1], (int)point[2], (int)point[3], (int)point[4], (int)point[5]);

    String jsonPacket2 = mac;
    // String jsonPacket2= to_string(printDeviceAddress());
    // printDeviceAddress();
    // Serial.println(jsonPacket2);
    jsonPacket= jsonPacket1 + jsonPacket2 + jsonPacket3;
    Serial.println(jsonPacket);
    // const char* jsonPacket = jsonPacket4;
    pDeviceCharacteristic->setValue(jsonPacket.c_str());
    pDeviceCharacteristic->notify();
  };

  void onDisconnect(BLEServer *pServer)
  {
    Serial.println("BLE : device disconnected");
    pServer->startAdvertising(); // restart advertising
  }
};

class ProvCharacteristicsCallbacks : public BLECharacteristicCallbacks
{
  
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0)
    {
      // read JSON data from Bluetooth
      StaticJsonDocument<128> doc;
      DeserializationError error = deserializeJson(doc, rxValue);
      if (error)
      {
        Serial.println("Error parsing JSON");
        return;
      }

      ssid = doc["ssid"];
      pass = doc["pass"];
      pWifiProv->storeWifiCredsRAM(ssid, pass);
      
      const char *masjidId = doc["masjid_id"];

      if (masjidId != NULL)
      {
        Serial.printf("Masjid ID : %s\n", masjidId);
      }

      if ((ssid != NULL) && (pass != NULL))
      {
        // switch to WiFi mode and connect
        pWifiProv->connectWifi(ssid, pass);
      }
      else
      {
        return;
      }        
    }
  }
};



bool BLE::initialize(WifiProvisioning* wifiProv)
{
  pWifiProv = wifiProv;
  WiFi.mode(WIFI_OFF);
  delay(100);
  // Create the BLE Device
  BLEDevice::init("SMBTN_SBB23V1");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID_TX,
      BLECharacteristic::PROPERTY_NOTIFY);

  pTxCharacteristic->addDescriptor(new BLE2902());

  pDeviceCharacteristic = pService->createCharacteristic(
                      DEVICE_CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pProvCharacteristic = pService->createCharacteristic(
      PROV_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_WRITE);

  pProvCharacteristic->setCallbacks(new ProvCharacteristicsCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");

  status = BLE_STARTED;
  return true;
}

bool BLE::stop(void)
{
  bool ret;
  ret = btStop();
  if (ret)
  {
    Serial.println("BLE Stopped"); 
    status = BLE_STOPPED;
  }
  return ret;
}

bool BLE::isStarted(void)
{
  return (status==BLE_STARTED);
}
 
void printDeviceAddress() {
 
  const uint8_t* point = esp_bt_dev_get_address();
 
  for (int i = 0; i < 6; i++) {
 
    char str[3];
 
    sprintf(str, "%02X", (int)point[i]);
    Serial.print(str);
 
    if (i < 5){
      Serial.print(":");
    }
 
  }
}

bool initBluetooth()
{
  if (!btStart()) {
    // Serial.println("Failed to initialize controller");
    return false;
  }
 
  if (esp_bluedroid_init() != ESP_OK) {
    // Serial.println("Failed to initialize bluedroid");
    return false;
  }
 
  if (esp_bluedroid_enable() != ESP_OK) {
    // Serial.println("Failed to enable bluedroid");
    return false;
  }
 
}



