// ============================================================
//  navigation.h  –  Beeline-style nav UI for SH1106 128x64
//  Random demo data: turn direction, distance, route curve
//  Call handleNavigation() from loop()
// ============================================================
#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <Arduino.h>
#include <U8g2lib.h>

// ── Extern display instance (defined in display.h) ───────────
extern U8G2_SH1106_128X64_NONAME_F_HW_I2C display;

// ── Turn directions ──────────────────────────────────────────
enum TurnDir {
  TURN_STRAIGHT,
  TURN_LEFT,
  TURN_RIGHT,
  TURN_SLIGHT_LEFT,
  TURN_SLIGHT_RIGHT,
  TURN_U_TURN
};

// ── Nav state ────────────────────────────────────────────────
struct NavState {
  TurnDir  turn;
  uint16_t distanceMetres;
  char     streetName[20];
};

NavState nav;
unsigned long lastNavUpdate = 0;

const char* demoStreets[] = {
  "MG Road", "Ring Road", "NH-48", "Link Road", "Station Rd"
};

// ── Randomise demo nav data every 3 seconds ──────────────────
void updateNavDemo() {
  unsigned long now = millis();
  if (now - lastNavUpdate < 3000) return;
  lastNavUpdate = now;

  nav.turn          = (TurnDir)random(0, 6);
  nav.distanceMetres = random(50, 900);
  strncpy(nav.streetName,
          demoStreets[random(0, 5)],
          sizeof(nav.streetName) - 1);
}

// ── Draw the circular border (Beeline style) ─────────────────
void drawCircleBorder() {
  // Outer ring centred at (64, 32), radius 30
  display.drawCircle(64, 32, 30);
  display.drawCircle(64, 32, 28);   // double ring for thickness
}

// ── Draw route curve in top half of circle ───────────────────
// Simulates the squiggly route line Beeline shows
void drawRouteCurve(TurnDir turn) {
  // Base: vertical line coming from bottom centre of circle
  // cx=64, bottom of circle ~y=58, top ~y=6
  // Draw a curved path depending on turn type

  switch (turn) {

    case TURN_STRAIGHT:
      // Simple straight line up
      display.drawVLine(64, 10, 30);
      // Arrow tip
      display.drawLine(64, 10, 59, 16);
      display.drawLine(64, 10, 69, 16);
      break;

    case TURN_RIGHT:
      // Curve: go up then bend right
      display.drawVLine(64, 24, 16);          // stem up
      display.drawHLine(64, 24, 10);          // bend right
      display.drawVLine(74, 10, 14);          // up on right
      // Arrow tip pointing right
      display.drawLine(74, 10, 68, 15);
      display.drawLine(74, 10, 68, 5);
      break;

    case TURN_LEFT:
      // Curve: go up then bend left
      display.drawVLine(64, 24, 16);          // stem up
      display.drawHLine(54, 24, 10);          // bend left
      display.drawVLine(54, 10, 14);          // up on left
      // Arrow tip pointing left
      display.drawLine(54, 10, 60, 15);
      display.drawLine(54, 10, 60, 5);
      break;

    case TURN_SLIGHT_RIGHT:
      // Gentle diagonal right
      display.drawVLine(64, 28, 12);
      display.drawLine(64, 28, 70, 14);
      display.drawVLine(70, 10, 4);
      display.drawLine(70, 10, 65, 15);
      display.drawLine(70, 10, 75, 15);
      break;

    case TURN_SLIGHT_LEFT:
      // Gentle diagonal left
      display.drawVLine(64, 28, 12);
      display.drawLine(64, 28, 58, 14);
      display.drawVLine(58, 10, 4);
      display.drawLine(58, 10, 53, 15);
      display.drawLine(58, 10, 63, 15);
      break;

    case TURN_U_TURN:
      // U shape
      display.drawVLine(60, 12, 18);          // left stem
      display.drawHLine(60, 30, 8);           // bottom
      display.drawVLine(68, 12, 18);          // right stem
      // Arrow pointing down on left stem
      display.drawLine(60, 12, 55, 18);
      display.drawLine(60, 12, 65, 18);
      break;
  }
}

// ── Draw small turn icon + distance in bottom half ───────────
void drawTurnInfo(TurnDir turn, uint16_t dist) {

  // ── Small turn arrow icon (left of distance) ─────────────
  // Centred around x=48, y=50
  switch (turn) {
    case TURN_STRAIGHT:
      display.drawVLine(48, 44, 12);
      display.drawLine(48, 44, 44, 49);
      display.drawLine(48, 44, 52, 49);
      break;
    case TURN_RIGHT:
      display.drawLine(44, 56, 44, 48);
      display.drawLine(44, 48, 52, 48);
      display.drawLine(52, 48, 48, 44);
      display.drawLine(52, 48, 48, 52);
      break;
    case TURN_LEFT:
      display.drawLine(52, 56, 52, 48);
      display.drawLine(52, 48, 44, 48);
      display.drawLine(44, 48, 48, 44);
      display.drawLine(44, 48, 48, 52);
      break;
    case TURN_SLIGHT_RIGHT:
      display.drawLine(44, 56, 50, 44);
      display.drawLine(50, 44, 54, 50);
      display.drawLine(50, 44, 44, 48);
      break;
    case TURN_SLIGHT_LEFT:
      display.drawLine(52, 56, 46, 44);
      display.drawLine(46, 44, 42, 50);
      display.drawLine(46, 44, 52, 48);
      break;
    case TURN_U_TURN:
      display.drawLine(46, 56, 46, 48);
      display.drawHLine(46, 48, 6);
      display.drawLine(52, 48, 52, 56);
      display.drawLine(52, 56, 48, 52);
      display.drawLine(52, 56, 56, 52);
      break;
  }

  // ── Distance number ───────────────────────────────────────
  display.setFont(u8g2_font_logisoso16_tf);
  char distStr[6];
  if (dist >= 1000) {
    snprintf(distStr, sizeof(distStr), "%d", dist / 1000);
  } else {
    snprintf(distStr, sizeof(distStr), "%d", dist);
  }
  display.drawStr(58, 62, distStr);

  // ── Unit label ────────────────────────────────────────────
  display.setFont(u8g2_font_5x7_tf);
  if (dist >= 1000) {
    display.drawStr(58, 52, "km");
  } else {
    display.drawStr(58, 52, "m");
  }
}

// ── Divider line across middle of circle ─────────────────────
void drawDivider() {
  // Horizontal line at y=36 clipped to circle width at that y
  // Circle: cx=64, cy=32, r=28 → at y=36 (dy=4): x_span=sqrt(28²-4²)≈27
  display.drawHLine(64 - 27, 36, 54);
}

// ── Street name at very bottom ────────────────────────────────
void drawStreetName(const char* name) {
  display.setFont(u8g2_font_4x6_tf);
  uint8_t w = display.getStrWidth(name);
  display.drawStr((128 - w) / 2, 64, name);
}

// ── Main draw ────────────────────────────────────────────────
void drawNavScreen() {
  display.clearBuffer();

  drawCircleBorder();
  drawRouteCurve(nav.turn);
  drawDivider();
  drawTurnInfo(nav.turn, nav.distanceMetres);
  drawStreetName(nav.streetName);

  display.sendBuffer();
}

// ── Call from loop() ─────────────────────────────────────────
void handleNavigation() {
  updateNavDemo();
  drawNavScreen();
}

#endif // NAVIGATION_H