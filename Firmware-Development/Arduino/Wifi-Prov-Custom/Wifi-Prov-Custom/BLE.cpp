#include "BLE.hpp"

WifiProvisioning* pWifiProv = NULL;

BLEServer *pServer = NULL;
BLEService *pService = NULL;
BLECharacteristic *pTxCharacteristic;
BLECharacteristic *pDeviceCharacteristic;
BLECharacteristic *pProvCharacteristic;
const char* jsonPacket = "{\"device_model\": \"SSB23V1\", \"mac_address\": \"abc:123:efg:456\", \"firmware_rev\": \"1.0\"}";
const char *ssid;
const char *pass;

class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    Serial.println("BLE : device connected");
    pDeviceCharacteristic->setValue(jsonPacket);
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
  BLEDevice::init("SMBTN_SSB23V1");

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
