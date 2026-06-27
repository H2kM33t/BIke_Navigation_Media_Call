// ============================================================
//  call_control.h  –  NimBLE HID Telephony Keys
//  Pin 32 = Answer | Pin 33 = Decline
//  Logic : INPUT_PULLDOWN, rising-edge (LOW->HIGH) detection
// ============================================================
#ifndef CALL_CONTROL_H
#define CALL_CONTROL_H

#include <Arduino.h>
#include <NimBLECharacteristic.h>

// ── Pin Definitions ──────────────────────────────────────────
const int Answer_Button  = 32;
const int Decline_Button = 33;

// ── Debounce States ──────────────────────────────────────────
static bool answerState  = LOW;
static bool declineState = LOW;

// ── HID Telephony Usage IDs (Usage Page 0x0B) ────────────────
#define HID_TELEPHONY_HOOK_SWITCH  0x0020
#define HID_TELEPHONY_RELEASE      0x0000

// ── Extern (defined in main.cpp) ─────────────────────────────
extern NimBLECharacteristic* callInputChar;

// ── Send key-down then key-up ────────────────────────────────
inline void sendCallKey(uint16_t usage) {
  if (!callInputChar) return;

  uint16_t down = usage;
  callInputChar->setValue((uint8_t*)&down, 2);
  callInputChar->notify();
  delay(30);

  uint16_t up = HID_TELEPHONY_RELEASE;
  callInputChar->setValue((uint8_t*)&up, 2);
  callInputChar->notify();
}

// ── Setup ─────────────────────────────────────────────────────
void setupCallPins() {
  pinMode(Answer_Button,  INPUT_PULLDOWN);
  pinMode(Decline_Button, INPUT_PULLDOWN);
  Serial.println("Call Control Pins Initialized");
}

// ── Handler – call every loop() ──────────────────────────────
void handleCallControl() {
  bool currentAnswer  = digitalRead(Answer_Button);
  bool currentDecline = digitalRead(Decline_Button);

  if (answerState == LOW && currentAnswer == HIGH) {
    Serial.println("Answer Call");
    sendCallKey(HID_TELEPHONY_HOOK_SWITCH);
  }

  if (declineState == LOW && currentDecline == HIGH) {
    Serial.println("Decline Call");
    sendCallKey(HID_TELEPHONY_HOOK_SWITCH);
  }

  answerState  = currentAnswer;
  declineState = currentDecline;
}

#endif // CALL_CONTROL_H