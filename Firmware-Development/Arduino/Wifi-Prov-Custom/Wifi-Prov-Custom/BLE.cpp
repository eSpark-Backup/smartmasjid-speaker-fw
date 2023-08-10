#include <Arduino.h>
#include <ArduinoJson.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "BLE.hpp"
#include "Device.hpp"
#include "SM_Json.hpp"
#include "SM_Nvs.hpp"

WifiProvisioning* pWifiProv = NULL;

BLEServer *pServer = NULL;
BLEService *pService = NULL;
BLECharacteristic *pTxCharacteristic;
BLECharacteristic *pDeviceCharacteristic;
BLECharacteristic *pProvCharacteristic;

class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    Serial.println("BLE : device connected");
    
    char deviceMacAddress[20];
    getDeviceMAC(deviceMacAddress);
    
    char factoryInfoJson[100];
    SM_Json::create(factoryInfoJson, sizeof(factoryInfoJson),
                        std::pair<const char*, const char*>("device_model", SM_MODEL),
                        std::pair<const char*, const char*>("mac_address", deviceMacAddress),
                        std::pair<const char*, const char*>("firmware_rev", SM_FIRMWARE_VER));                                         
    Serial.println(factoryInfoJson);
    
    pDeviceCharacteristic->setValue(factoryInfoJson);
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
      char masjidId[32];
      if (SM_Json::parse(rxValue.c_str(), std::make_pair("masjid_id", masjidId))) 
      {
        Serial.printf("Masjid id : %s\n", masjidId);
        // Store masjid id in NVS
          Sm_Nvs::put(DEVICE_FACTORY_INFO_NAMESPACE,
                      std::make_pair("masjidId", masjidId));
      }
      else
      {
        Serial.println("Unable to parse masjis id");
      }  
      
      char ssid[32];
      char pass[32];
      if (SM_Json::parse(rxValue.c_str(), std::make_pair("ssid", ssid), std::make_pair("pass", pass))) 
      {
        pWifiProv->storeWifiCredsRAM(ssid, pass);
        pWifiProv->connectWifi(ssid, pass);
      } 
      else
      {
        Serial.println("Unable to parse wifi credentials");
      }     
    }
  }
};

BLE::BLE(WifiProvisioning* wifiProv)
{
  pWifiProv = wifiProv;
}

bool BLE::initialize()
{
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
