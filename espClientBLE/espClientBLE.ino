#include <BLEDevice.h>
#include <BLEClient.h>

// The remote service we wish to connect to
static BLEUUID serviceUUID("19B10000-E8F2-537E-4F6C-D104768A1214");
// The characteristic of the remote service we are interested in
static BLEUUID charUUID("19B10001-E8F2-537E-4F6C-D104768A1214");

static BLEClient* pClient;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static boolean connected = false;

// Callback function for connection events
class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
    connected = true;
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("Disconnected");
  }
};

void setup() {
  Serial.begin(115200);
  
  // Initialize BLE device
  BLEDevice::init("ESP32");
  
  // Create the BLE Client
  pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());
  
  // Connect to the remote BLE Server
  connectToServer();
}

void loop() {
  if (Serial.available()) {
    char inChar = Serial.read();
    if (connected && pRemoteCharacteristic) {
      // Forward the received value to the Nano 33 BLE
      pRemoteCharacteristic->writeValue(&inChar, 1);
      Serial.println("Value sent to Nano 33 BLE: " + String(inChar));
    }
  }

  // If disconnected, try to reconnect
  if (!connected) {
    connectToServer();
  }
}

bool connectToServer() {
  Serial.println("Scanning for BLE servers...");
  
  // Scan for the BLE server
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setActiveScan(true);
  
  // Updated for ESP32 Arduino core v3.0.0+
  int scanTime = 5; //In seconds
  BLEScanResults* foundDevices = pBLEScan->start(scanTime, false);
  
  // Updated count retrieval
  Serial.print("Devices found: ");
  Serial.println(foundDevices->getCount());
  
  // Find the device with the correct service UUID
  for(int i = 0; i < foundDevices->getCount(); i++) {
    BLEAdvertisedDevice device = foundDevices->getDevice(i);
    if (device.haveServiceUUID() && device.getServiceUUID().equals(serviceUUID)) {
      Serial.println("Found our device!");
      
      // Connect to the found device
      pClient->connect(&device);
      
      // Obtain a reference to the service we are after
      BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
      if (pRemoteService == nullptr) {
        Serial.println("Failed to find service");
        return false;
      }
      
      // Obtain a reference to the characteristic
      pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
      if (pRemoteCharacteristic == nullptr) {
        Serial.println("Failed to find characteristic");
        return false;
      }
      
      // If you need to read values, use String instead of std::string
      // String value = pRemoteCharacteristic->getValue();
      
      pBLEScan->clearResults();   // Delete results from BLEScan buffer to release memory
      return true;
    }
  }
  
  pBLEScan->clearResults();   // Delete results from BLEScan buffer to release memory
  return false;
}