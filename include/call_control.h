#ifndef CALL_CONTROL_H
#define CALL_CONTROL_H

#include <BleKeyboard.h>

// ── Pin Definitions ──────────────────────────────────────────
const int Accept_Button  = 33; //ACCEPT BUTTON
const int Decline_Button = 32; //DECLINE BUTTON

// ── Debounce States ──────────────────────────────────────────
bool acceptState  = LOW; //By Defualt LOW 
bool declineState = LOW; //By Defualt LOW 

// ── Call Control Keys ────────────────────────────────────────
// Uses consumer key codes for call accept/decline
#define KEY_CALL_ACCEPT  0x00B0   // HID telephony accept call
#define KEY_CALL_DECLINE 0x00B1   // HID telephony decline call

// ── Function ─────────────────────────────────────────────────
void handleCallControl(BleKeyboard &bleKeyboard) {

  bool currentAccept  = digitalRead(Accept_Button);
  bool currentDecline = digitalRead(Decline_Button);

  // Accept Call
  if (currentAccept == HIGH && acceptState == LOW) {
    Serial.println("Call Accepted");
    bleKeyboard.write(KEY_CALL_ACCEPT);
    delay(50);
  }

  // Decline Call
  if (currentDecline == HIGH && declineState == LOW) {
    Serial.println("Call Declined");
    bleKeyboard.write(KEY_CALL_DECLINE);
    delay(50);
  }

  // Save states
  acceptState  = currentAccept;
  declineState = currentDecline;
}

// ── Setup Function ───────────────────────────────────────────
void setupCallPins() {
  pinMode(Accept_Button,  INPUT_PULLDOWN);
  pinMode(Decline_Button, INPUT_PULLDOWN);
  Serial.println("Call Control Pins Initialized");
}

#endif // CALL_CONTROL_H
