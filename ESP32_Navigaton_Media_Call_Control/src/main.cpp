#include <Arduino.h>
#include <BleKeyboard.h>

// ── Pin Definitions ──────────────────────────────────────────
const int Prev_Button      = 27;
const int PlayPause_Button = 26;
const int Next_Button      = 25;

// ── BLE Keyboard Instance ────────────────────────────────────
// BleKeyboard(deviceName, manufacturer, battery%)
BleKeyboard bleKeyboard("ESP32 Media Control", "ESP32", 100);

// ── Debounce State ───────────────────────────────────────────
bool prevState      = LOW;
bool playPauseState = LOW;
bool nextState      = LOW;

void setup() {
  Serial.begin(115200);

  pinMode(Prev_Button,      INPUT_PULLDOWN);
  pinMode(PlayPause_Button, INPUT_PULLDOWN);
  pinMode(Next_Button,      INPUT_PULLDOWN);

  bleKeyboard.begin();
  Serial.println("BLE Keyboard started. Waiting for connection...");
}

void loop() {
  if (!bleKeyboard.isConnected()) {
    Serial.println("Waiting for BLE connection...");
    delay(1000); // Don't spam serial while disconnected
    return;
  }

  // ── Read current button states ───────────────────────────
  bool currentPrev      = digitalRead(Prev_Button);
  bool currentPlayPause = digitalRead(PlayPause_Button);
  bool currentNext      = digitalRead(Next_Button);

  // ── Previous Track ───────────────────────────────────────
  if (currentPrev == HIGH && prevState == LOW) {       // Rising edge only
    Serial.println("⏮ Previous Track");
    bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
    delay(50); // Debounce
  }

  // ── Play / Pause ─────────────────────────────────────────
  if (currentPlayPause == HIGH && playPauseState == LOW) {
    Serial.println("⏯ Play/Pause");
    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
    delay(50);
  }

  // ── Next Track ───────────────────────────────────────────
  if (currentNext == HIGH && nextState == LOW) {
    Serial.println("⏭ Next Track");
    bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
    delay(50);
  }

  // ── Save states for edge detection next loop ─────────────
  prevState      = currentPrev;
  playPauseState = currentPlayPause;
  nextState      = currentNext;
}