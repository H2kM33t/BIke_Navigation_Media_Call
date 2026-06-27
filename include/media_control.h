// ============================================================
//  media_control.h  –  NimBLE HID Consumer Media Keys
//  Pin 27 = Previous | Pin 26 = Play/Pause | Pin 25 = Next
//  Logic : INPUT_PULLDOWN, rising-edge (LOW->HIGH) detection
// ============================================================
#ifndef MEDIA_CONTROL_H
#define MEDIA_CONTROL_H

#include <Arduino.h>
#include <NimBLECharacteristic.h>

// ── Pin Definitions ──────────────────────────────────────────
const int Prev_Button      = 27;
const int PlayPause_Button = 26;
const int Next_Button      = 25;

// ── Debounce States ──────────────────────────────────────────
static bool prevState      = LOW;
static bool playPauseState = LOW;
static bool nextState      = LOW;

// ── HID Consumer Usage IDs (Usage Page 0x0C) ─────────────────
#define HID_CONSUMER_NEXT_TRACK   0x00B5
#define HID_CONSUMER_PREV_TRACK   0x00B6
#define HID_CONSUMER_PLAY_PAUSE   0x00CD
#define HID_CONSUMER_RELEASE      0x0000

// ── Extern (defined in main.cpp) ─────────────────────────────
extern NimBLECharacteristic* mediaInputChar;

// ── Send key-down then key-up ────────────────────────────────
inline void sendMediaKey(uint16_t usage) {
  if (!mediaInputChar) return;

  uint8_t report[2];

  // Key down
  report[0] = usage & 0xFF;
  report[1] = (usage >> 8) & 0xFF;
  mediaInputChar->setValue(report, 2);
  mediaInputChar->notify();
  delay(50);

  // Key up
  report[0] = 0x00;
  report[1] = 0x00;
  mediaInputChar->setValue(report, 2);
  mediaInputChar->notify();
  delay(20);
}

// ── Setup ─────────────────────────────────────────────────────
void setupMediaPins() {
  pinMode(Prev_Button,      INPUT_PULLDOWN);
  pinMode(PlayPause_Button, INPUT_PULLDOWN);
  pinMode(Next_Button,      INPUT_PULLDOWN);
  Serial.println("Media Control Pins Initialized");
}

// ── Handler – call every loop() ──────────────────────────────
void handleMediaControl() {
  bool currentPrev      = digitalRead(Prev_Button);
  bool currentPlayPause = digitalRead(PlayPause_Button);
  bool currentNext      = digitalRead(Next_Button);

  if (prevState == LOW && currentPrev == HIGH) {
    Serial.println("Previous Track");
    sendMediaKey(HID_CONSUMER_PREV_TRACK);
  }

  if (playPauseState == LOW && currentPlayPause == HIGH) {
    Serial.println("Play/Pause");
    sendMediaKey(HID_CONSUMER_PLAY_PAUSE);
  }

  if (nextState == LOW && currentNext == HIGH) {
    Serial.println("Next Track");
    sendMediaKey(HID_CONSUMER_NEXT_TRACK);
  }

  prevState      = currentPrev;
  playPauseState = currentPlayPause;
  nextState      = currentNext;
}

#endif // MEDIA_CONTROL_H