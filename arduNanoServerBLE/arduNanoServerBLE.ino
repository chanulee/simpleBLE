#include <ArduinoBLE.h>

BLEService numberService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEByteCharacteristic numberCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", 
                                         BLERead | BLEWrite | BLENotify);

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  if (!BLE.begin()) {
    Serial.println("BLE initialization failed!");
    while (1);
  }

  BLE.setLocalName("Nano33BLE");
  BLE.setAdvertisedService(numberService);
  numberService.addCharacteristic(numberCharacteristic);
  BLE.addService(numberService);
  numberCharacteristic.writeValue(0);
  BLE.advertise();
  Serial.println("BLE server started, waiting for connections...");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      if (numberCharacteristic.written()) {
        byte value = numberCharacteristic.value();
        
        // Print the received value in different formats for debugging
        Serial.print("Received value (decimal): ");
        Serial.println(value);
        Serial.print("Received value (char): ");
        Serial.println((char)value);
        
        // Handle toggle values (0/1)
        if (value == '0' || value == '1') {
          digitalWrite(LED_BUILTIN, value == '1' ? HIGH : LOW);
          Serial.println(value == '1' ? "LED ON" : "LED OFF");
        }
        // Handle numeric values (1-8)
        else if (value >= '1' && value <= '8') {
          int numValue = value - '0';  // Convert ASCII to actual number
          Serial.print("Received number: ");
          Serial.println(numValue);
          // Here you can add specific actions for numbers 1-8
          // For example, different blink patterns
          blinkLED(numValue);
        }
      }
    }
    Serial.println("Disconnected from central");
  }
}

// Function to blink LED based on number received
void blinkLED(int times) {
  for(int i = 0; i < times; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
}