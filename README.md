# simpleBLE
BLE client &amp; server communication with ESP32 and Arduino Nano BLE 33.

Processing --Serial Communication--> ESP32 (Client) --BLE Communication--> Arduino Nano BLE 33 (Server)

- ESP32 BLE library (installed within ESP library) and ArduinoBLE library (search library manager) crashes for ESP32 project. So upload to Arduino Nano BLE 33 first, then remove the ArduinoBLE library from library manager and then upload to ESP32.
- Close all the serial monitors before opening the processing app.
- Windoes OS won't need this as it's easy to directly connect to BLE devices and send message.
- Refer to lightBlue app on mac appstore for initial testing of BLE of each boards.
- UUID generation at https://www.uuidgenerator.net/version4
- For ESP 3.0.0 and higher, refer to https://wiki.seeedstudio.com/XIAO_ESP32C3_Bluetooth_Usage/ for errors (already applied to the code)
