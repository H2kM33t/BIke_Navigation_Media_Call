#include <Arduino.h>
#include <BleKeyboard.h>
#include "media_control.h"
#include "call_control.h"

// BLE Keyboard Instance
BleKeyboard bleKeyboard("ESP32 Media Control", "ESP32", 100);

bool wasConnected = false;

void setup() {
  Serial.begin(115200);

  setupMediaPins();   // From media_control.h
  setupCallPins();    // From call_control.h

  bleKeyboard.begin();
  Serial.println("BLE Keyboard Started. Waiting for connection...");
}

void loop() {
  bool connected = bleKeyboard.isConnected();

  if (!connected) {
    wasConnected = false;
    Serial.println("Waiting for BLE connection...");
    delay(1000);
    return;
  }

  if (!wasConnected) {
    Serial.println("BLE connection Established");
    wasConnected = true;
  }

  handleMediaControl(bleKeyboard);  // From media_control.h
  handleCallControl(bleKeyboard);   // From call_control.h

  delay(20);
}