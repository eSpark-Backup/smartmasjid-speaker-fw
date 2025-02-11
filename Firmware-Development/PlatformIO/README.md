# PlatformIO Setup Guide

This guide provides instructions for setting up PlatformIO for developing and managing your project.

## Prerequisites

Before starting, ensure you have the following installed on your system:
- [Visual Studio Code (VS Code)](https://code.visualstudio.com/)
- Python (version 3.6 or later)
- PlatformIO Core CLI (optional but recommended for advanced users)

## Installation Steps

1. **Install PlatformIO Extension for VS Code**  
   - Open VS Code.
   - Go to the **Extensions** view (`Ctrl+Shift+X` or `Cmd+Shift+X` on macOS).
   - Search for `PlatformIO IDE` and click **Install**.

2. **Verify Installation**  
   - Open the PlatformIO Home screen by clicking on the PlatformIO icon in the Activity Bar.
   - Ensure PlatformIO initializes correctly and loads its dashboard.

3. **Open an Existing PlatformIO Project (Platformio-Firmware)**  
   - To open an existing project:  
     1. Go to **PlatformIO Home > Open Project**.  
     2. Navigate to the folder containing your PlatformIO project.  
     3. Select the folder and click **Open**.  
   - Alternatively, you can use **File > Open Folder** in VS Code and select your project folder.  


4. **Configure the Existing `platformio.ini` File**  
   - Open the `platformio.ini` file located in the root directory of your project.  
   - This file is already created when you set up your project and contains default settings for your selected board and framework.  
   - Press (Cntrl + S) to download the packages and libraries. 
   - After Download and configuration.

## Building and Uploading Code

1. **Build the Project**  
   - Click on the checkmark icon in the PlatformIO toolbar:
    ![image](https://github.com/user-attachments/assets/836e29f1-1e3d-4dfb-a6fc-c41b6e6e37e8)

2. **Upload Code to the Board**  
   - Click on the arrow icon in the PlatformIO toolbar:
   ![image](https://github.com/user-attachments/assets/70e42001-052f-43bf-bb5c-5261d3da1c30)

3. **Monitor Serial Output**  
   - Use the plug icon in the PlatformIO toolbar:
   ![image](https://github.com/user-attachments/assets/dc8b94d6-c488-4a72-b386-f00b21382cf8)

## Common Issues and Troubleshooting

- **Detected a whitespace character in project paths**:  
  Ensure your project path does not contain spaces. Move the project to a location without spaces, e.g., `C:\Projects\MyProject`.

- **Board Not Detected**:  
  Check that the correct port is selected in the `platformio.ini` file.  
  Example:
  ```ini
  upload_port = COM3

## Upload Binaries on ESP32:
Follow these steps to upload the binary files to the ESP32 through `flash_download_tool`:
### Required Software
1. [ESP32 Flash Download Tool](https://www.espressif.com/sites/default/files/tools/flash_download_tool_3.9.4.zip)

### Testing procedure
1. Connect the ESP32 Dev Module to the PC via MicroUSB cable.
2. Open flash download tool and select *Chip Type* as *ESP32* and *WorkMode* as *Develop*. Press *OK* to move to the next screen. 
3. Select correct *COM Port* and set *Baud rate* to *115200* and then click erase button on new window that opens, to first completely wipe off the ESP32 memory.
4. Upload the provided binaries as follows:
   ```
      * bootloader.bin  OR  bootloader.bin -> 0x1000
      * partitions.bin  OR  partitions.bin -> 0x8000
      * firmware.bin    OR  firmware.bin   -> 0x10000
      * spiffs.bin      OR  spiffs.bin    -> 0x310000
    ```
5. Press the start button to start the flashing process.
6. Once finished, close the software and reset the ESP32 using EN button on the Dev Module. For the custom board, you can power cycle the board (turning power ON/OFF)
7. Open termite serial terminal and connect to ESP32 serial port at baud rate of 115200 in order to observe console log.
8. The ESP32 will first go into WiFi mode and search its memory for stored WiFi credentials, failing to find any, it will fallback to BLE mode.
9. Supply the WiFi credentials using Smart Masjid App or alternatively nRF Connect app at READ characteristic in JSON format as below:  
 { "ssid":"string", "pass":"string", "masjid_id":"string" }
10. After receiving, the device will initiate connection to internet and after successful connection, it will hit an API to tell the cloud its online and save provided credentials in EEPROM (permanent memory)
11. For another cycle of testing, repeat all the above steps.
12. Demo video link:
    [Upload Binaries (Demo video](https://www.loom.com/share/412dfff911c84de7bd726a3a6ad232f1?sid=ba3c8dc7-31e9-4fb5-bad0-06bdca803d01)


