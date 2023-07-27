#pragma once
#include "WifiProvisioning.hpp"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
bool initBluetooth();

#define SERVICE_UUID                "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_TX      "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
#define DEVICE_CHARACTERISTIC_UUID  "6E400010-B5A3-F393-E0A9-E50E24DCCA9E"
#define PROV_CHARACTERISTIC_UUID  "6E400011-B5A3-F393-E0A9-E50E24DCCA9E"
void printDeviceAddress(void);
bool initBluetooth();
typedef enum 
{
  BLE_STOPPED,
  BLE_STARTED,
} bleStatus;

class BLE
{
  private:
    bleStatus status = BLE_STOPPED;
    
  public:
    bool initialize(WifiProvisioning* wifiProv);
    bool stop(void);
    bool isStarted(void);
};
