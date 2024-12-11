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
   - Click on the checkmark icon in the PlatformIO toolbar or run:
 ![image](https://github.com/user-attachments/assets/77029463-3997-4d0c-88e3-48e3b09b0dc7)

OR to run `platformio run` in platformio terminal to compile. 
![image](https://github.com/user-attachments/assets/706e8800-ad06-481c-be83-514b12930aad)

2. **Upload Code to the Board**  
   - Click on the arrow icon in the PlatformIO toolbar or run:
![image](https://github.com/user-attachments/assets/4a7f7f9b-5170-4699-94e1-b80366b19cc0)
OR to run `platformio run --target upload` in platformio terminal to upload. 

3. **Monitor Serial Output**  
   - Use the plug icon in the PlatformIO toolbar or run:
     ```bash
     platformio device monitor
     ```
## Common Issues and Troubleshooting

- **Detected a whitespace character in project paths**:  
  Ensure your project path does not contain spaces. Move the project to a location without spaces, e.g., `C:\Projects\MyProject`.

- **Board Not Detected**:  
  Check that the correct port is selected in the `platformio.ini` file.  
  Example:
  ```ini
  upload_port = COM3


