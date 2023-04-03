#include "BLE.hpp"

BLEServer *pServer = NULL;
BLEService *pService = NULL;
BLECharacteristic *pTxCharacteristic;

class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    Serial.println("BLE : device connected");
  };

  void onDisconnect(BLEServer *pServer)
  {
    Serial.println("BLE : device disconnected");
    pServer->startAdvertising(); // restart advertising
  }
};

class DeviceCharacteristicsCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0)
    {
      Serial.print("Dev Received Value: ");
      Serial.println(rxValue.c_str());
    }
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
      }

      const char *ssid = doc["ssid"];
      const char *password = doc["password"];

      preferences.putString("ssid", ssid);
      preferences.putString("password", password);

      // switch to WiFi mode and connect
      if (connectWifi(ssid, password))
      {
        pServer->removeService(pService);
        BLEDevice::deinit();
      }
    }
  }
};

bool initializeBLE(void)
{
  WiFi.mode(WIFI_OFF);
  // Create the BLE Device
  BLEDevice::init("MYESP32");

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

  BLECharacteristic *pDeviceCharacteristic = pService->createCharacteristic(
      DEVICE_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_WRITE);

  pDeviceCharacteristic->setCallbacks(new DeviceCharacteristicsCallbacks());

  BLECharacteristic *pProvCharacteristic = pService->createCharacteristic(
      PROV_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_WRITE);

  pProvCharacteristic->setCallbacks(new ProvCharacteristicsCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
  return true;
}
