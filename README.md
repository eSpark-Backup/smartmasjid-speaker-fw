# Smart Masjid - Firmware
 To develop firmware for IoT Button and IoT Speaker products

## API Swagger Documentation
[Swagger](http://smartmasjid-api-qa.futuriklabs.com/swagger-ui/index.html#/)

## Firmware Flowchart
[DrawIO Flowchart](https://app.diagrams.net/#G1D794t6WH5X4qnHniXaRM_4V8VDZjGsNo#%7B%22pageId%22%3A%22XOtjJPAAMMDfJb9Jv2DV%22%7D)

## Testing BLE Provisioning with ESP32 Dev Module
### Required Software
1. [ESP32 Flash Download Tool](https://www.espressif.com/sites/default/files/tools/flash_download_tool_3.9.4.zip)

### Testing procedure
1. Connect the ESP32 Dev Module to the PC via MicroUSB cable.
2. Open flash download tool and select options as shown in screenshot below and click OK to proceed  
![image](https://user-images.githubusercontent.com/76909080/230549261-5fc7d44c-2a23-4bef-be14-1111a2dfd05e.png)
3. Click erase button on new window that opens, to first completely wipe off the ESP32 memory.
3. Upload the provided [binaries](https://github.com/eSpark-Consultants/smartmasjid-firmware/tree/Development/Binaries) as follows:
   * Wifi-Prov-Custom.ino.bootloader.bin -> 0x1000
   * Wifi-Prov-Custom.ino.partitions.bin -> 0x8000
   * Wifi-Prov-Custom.ino.bin            -> 0x10000
4. Press the start button to start the flashing process as shown in screenshot below:
![image](https://user-images.githubusercontent.com/76909080/230550704-18c84a62-4963-4835-9ee2-a672fda23945.png)
5. Once finished, close the software and reset the ESP32 using EN button on the Dev Module.
6. Open termite serial terminal and connect to ESP32 serial port at baud rate of 115200 in order to observe console log.
7. The ESP32 will first go into WiFi mode and search its memory for stored WiFi credentials, failing to find any, it will fallback to BLE mode.
8. Supply the WiFi credentials using nRF Connect app at READ characteristic in JSON format as below:  
 { "ssid":"string", "pass":"string", "masjid_id":"string" }
9. After receiving, the device will initiate connection to internet and after successful connection, it will hit an API to tell the cloud its online and save provided credentials in EEPROM (permanent memory)
10. For another cycle of testing, repeat all the above steps.

## ESP32 Board and Libreries
1. Esp32 Board Version     (Version : 2.0.6)
2. PubSubClient            (Version : 2.8)
3. WiFi                    (Version : 0.0.3)
4. WiFiClientSecure        (Version : 1.0.0)
5. ArduinoJson             (Version : 6.21.2)
