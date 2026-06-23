#ifndef MEDIA_CONTROL_H
#define MEDIA_CONTROL_H

#include <BleKeyboard.h>

// ── Pin Definitions ──────────────────────────────────────────
const int Prev_Button      = 27;
const int PlayPause_Button = 26;
const int Next_Button      = 25;

// ── Debounce States ──────────────────────────────────────────
bool prevState      = LOW;
bool playPauseState = LOW;
bool nextState      = LOW;

// ── Function ─────────────────────────────────────────────────
void handleMediaControl(BleKeyboard &bleKeyboard) {

  bool currentPrev      = digitalRead(Prev_Button);
  bool currentPlayPause = digitalRead(PlayPause_Button);
  bool currentNext      = digitalRead(Next_Button);

  // Previous Track
  if (currentPrev == HIGH && prevState == LOW) {
    Serial.println("Previous Track");
    bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
    delay(50);
  }

  // Play/Pause
  if (currentPlayPause == HIGH && playPauseState == LOW) {
    Serial.println("Play/Pause");
    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
    delay(50);
  }

  // Next Track
  if (currentNext == HIGH && nextState == LOW) {
    Serial.println("Next Track");
    bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
    delay(50);
  }

  // Save states
  prevState      = currentPrev;
  playPauseState = currentPlayPause;
  nextState      = currentNext;
}

// ── Setup Function ───────────────────────────────────────────
void setupMediaPins() {
  pinMode(Prev_Button,      INPUT_PULLDOWN);
  pinMode(PlayPause_Button, INPUT_PULLDOWN);
  pinMode(Next_Button,      INPUT_PULLDOWN);
  Serial.println("Media Control Pins Initialized");
}

#endif // MEDIA_CONTROL_H