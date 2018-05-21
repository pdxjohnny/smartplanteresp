# smartplanteresp

## Arduino IDE Environment Setup
Version: 1.8.5

1. Go to File > Preferences. In "Additional Boards Manager URLs", enter http://arduino.esp8266.com/stable/package_esp8266com_index.json
2. Go to Tools > Boards: xxx > Boards Manager... Install "esp 8266 by ESP8266 Community" version 2.4.1.
3. Go to Sketch > Include Libraries > Library Manager. Install "ArduinoJson by Benoit Blanchon" Version 5.13.1
4. Under Tool, make sure the following is selected:

    Name          | Value
    ------------- | -------------------------------
    Board         | "NodeMCU 1.0 (ESP-12E Module)"
    Flash Size    | "4M (3M 3PIFFS)"
    Debug port    | "Disabled"
    Debug Level   | "None"
    IwIP Variant  | "v2 Lower Memory"
    CPU Frequency | "80 MHz"
    Upload Speed  | "921600"
    Erase Flash   | "All Flash Contents"
    Port          | "COMx*"

    Note: *x is the COM number to which the NodeMCU is connected. This can be found in Device Managers > Ports. Look for data entry "Silicon Labs CP210x USB to UART Bridge." If this is not displayed. Install the driver for CP210x.
    
5. Refer to WiFiManager Library section to install WiFiManager Library
    
### CP210x Driver
The driver for CP210x may not be automatically installed when connected to a computer. Verify that the device driver is properly loaded in Device Manager. If it is not automatically installed, the driver can be found here. https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers

## WiFiManager Library

Mixed content policy only allows passive resources to load over HTTP
requests. This is a way we can circumvent that policy by requesting a
"passive resource", an Image load, which the server running on the
planter will accept and use the parameters to connect to WiFi.
[Mixed content](https://developer.mozilla.org/en-US/docs/Web/Security/Mixed_content)

To do this we had to modify WiFiManager so that it returns an image.
Therefore to setup this project you will have to have the branch we modified
checked out in your Arduino liraries directory.

### Linux

```console
git clone https://github.com/pdxjohnny/WiFiManager \
  -b cors_all ~/Arduino/libraries/WiFiManager
```

### Windows (git bash)

```console
git clone https://github.com/pdxjohnny/WiFiManager \
  -b cors_all ~/Documents/Arduino/libraries/WiFiManager
```
