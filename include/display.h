// ============================================================

//  display.h  –  SH1106 1.3" OLED via U8g2

//  I2C : SDA=21, SCL=22 (ESP32 default)

//  Shows random demo data: BLE status, track, speed, heart rate

// ============================================================

#ifndef DISPLAY_H

#define DISPLAY_H

 

#include <Arduino.h>

#include <U8g2lib.h>

#include <Wire.h>

 

// ── U8g2 Instance ────────────────────────────────────────────

// SH1106 128x64, I2C, full buffer, no reset pin

U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

 

// ── Random Demo Data ─────────────────────────────────────────

struct DisplayData {

  bool      bleConnected;

  char      trackName[24];

  uint8_t   speed;       // km/h

  uint8_t   heartRate;   // bpm

  uint8_t   battery;     // %

};

 

DisplayData demoData;

unsigned long lastUpdate = 0;

 

// ── Track name pool for random demo ──────────────────────────

const char* demoTracks[] = {

  "Bohemian Rhapsody",

  "Blinding Lights",

  "Shape of You",

  "Stay With Me",

  "Levitating"

};

const uint8_t TRACK_COUNT = 5;

 

// ── Setup ─────────────────────────────────────────────────────

void setupDisplay() {

  Wire.begin(21, 22);

  display.begin();

  display.setContrast(200);

  Serial.println("OLED Display Initialized");

}

 

// ── Generate random demo values every 2 seconds ──────────────

void updateDemoData(bool bleConnected) {

  unsigned long now = millis();

  if (now - lastUpdate < 2000) return;

  lastUpdate = now;

 

  demoData.bleConnected = bleConnected;

  demoData.speed        = random(15, 42);          // 15–42 km/h

  demoData.heartRate    = random(110, 175);         // 110–175 bpm

  demoData.battery      = random(60, 100);          // 60–100 %

  strncpy(demoData.trackName,

          demoTracks[random(0, TRACK_COUNT)],

          sizeof(demoData.trackName) - 1);

}

 

// ── Draw battery icon ─────────────────────────────────────────

void drawBattery(uint8_t x, uint8_t y, uint8_t pct) {

  // Outer box 18x9

  display.drawFrame(x, y, 18, 9);

  // Battery tip

  display.drawBox(x + 18, y + 2, 2, 5);

  // Fill based on %

  uint8_t fill = (uint8_t)((pct / 100.0f) * 16);

  if (fill > 0) display.drawBox(x + 1, y + 1, fill, 7);

}

 

// ── Main draw function ────────────────────────────────────────

void drawDisplay() {

  display.clearBuffer();

 

  // ── Top bar ──────────────────────────────────────────────

  // BLE status dot

  if (demoData.bleConnected) {

    display.drawDisc(4, 4, 3);          // filled = connected

  } else {

    display.drawCircle(4, 4, 3);        // outline = disconnected

  }

 

  // Device name

  display.setFont(u8g2_font_6x10_tf);

  display.drawStr(10, 9, "Bike Control");

 

  // Battery top-right

  drawBattery(106, 0, demoData.battery);

 

  // Divider line

  display.drawHLine(0, 12, 128);

 

  // ── Speed (large, centre) ────────────────────────────────

  display.setFont(u8g2_font_logisoso28_tf);

  char speedStr[6];

  snprintf(speedStr, sizeof(speedStr), "%d", demoData.speed);

  uint8_t speedW = display.getStrWidth(speedStr);

  display.drawStr((128 - speedW) / 2, 48, speedStr);

 

  // km/h label

  display.setFont(u8g2_font_6x10_tf);

  display.drawStr(52, 58, "km/h");

 

  // ── Heart rate (bottom left) ─────────────────────────────

  char hrStr[8];

  snprintf(hrStr, sizeof(hrStr), "%d bpm", demoData.heartRate);

  display.drawStr(0, 64, hrStr);

 

  // ── Track name (scrolling ticker at top) ─────────────────

  display.setFont(u8g2_font_5x7_tf);

  // Scroll offset based on time

  int offset = (millis() / 80) % (strlen(demoData.trackName) * 6 + 128);

  display.drawStr(128 - offset, 23, demoData.trackName);

 

  // Divider above track

  display.drawHLine(0, 14, 128);

 

  display.sendBuffer();

}

 

// ── Call from loop() ──────────────────────────────────────────

void handleDisplay(bool bleConnected) {

  updateDemoData(bleConnected);

  drawDisplay();

}

 

#endif // DISPLAY_H