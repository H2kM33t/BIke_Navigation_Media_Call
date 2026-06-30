// // ============================================================

// //  navigation.h  –  Beeline-style nav UI for SH1106 128x64

// //  Random demo data: turn direction, distance, route curve

// //  Call handleNavigation() from loop()

// // ============================================================
// #ifndef NAVIGATION_H
// #define NAVIGATION_H

// #include <Arduino.h>
// #include <U8g2lib.h>

// // ── Extern display instance (defined in display.h) ───────────

// extern U8G2_SH1106_128X64_NONAME_F_HW_I2C display;

// // ── Turn directions ──────────────────────────────────────────

// enum TurnDir
// {

//   TURN_STRAIGHT,

//   TURN_LEFT,

//   TURN_RIGHT,

//   TURN_SLIGHT_LEFT,

//   TURN_SLIGHT_RIGHT,

//   TURN_U_TURN

// };
// // ── Nav state ────────────────────────────────────────────────

// struct NavState
// {

//   TurnDir turn;

//   uint16_t distanceMetres;

//   char streetName[20];
// };

// NavState nav;

// unsigned long lastNavUpdate = 0;

// const char *demoStreets[] = {

//     "MG Road", "Ring Road", "NH-48", "Link Road", "Station Rd"

// };

// // ── Randomise demo nav data every 3 seconds ──────────────────

// void updateNavDemo()
// {
//   unsigned long now = millis();
//   if (now - lastNavUpdate < 3000)
//     return;
//   lastNavUpdate = now;

//   nav.turn = (TurnDir)random(0, 6);

//   nav.distanceMetres = random(50, 900);

//   strncpy(nav.streetName,

//           demoStreets[random(0, 5)],

//           sizeof(nav.streetName) - 1);
// }

// // ── Draw the circular border (Beeline style) ─────────────────

// void drawCircleBorder()
// {

//   // Outer ring centred at (64, 32), radius 30

//   display.drawCircle(64, 32, 30);

//   display.drawCircle(64, 32, 28); // double ring for thickness
// }

// // ── Draw route curve in top half of circle ───────────────────

// // Simulates the squiggly route line Beeline shows

// void drawRouteCurve(TurnDir turn)
// {

//   // Base: vertical line coming from bottom centre of circle

//   // cx=64, bottom of circle ~y=58, top ~y=6

//   // Draw a curved path depending on turn type

//   switch (turn)
//   {

//   case TURN_STRAIGHT:

//     // Simple straight line up

//     display.drawVLine(64, 10, 30);

//     // Arrow tip

//     display.drawLine(64, 10, 59, 16);

//     display.drawLine(64, 10, 69, 16);

//     break;

//   case TURN_RIGHT:

//     // Curve: go up then bend right

//     display.drawVLine(64, 24, 16); // stem up

//     display.drawHLine(64, 24, 10); // bend right

//     display.drawVLine(74, 10, 14); // up on right

//     // Arrow tip pointing right

//     display.drawLine(74, 10, 68, 15);

//     display.drawLine(74, 10, 68, 5);

//     break;

//   case TURN_LEFT:

//     // Curve: go up then bend left

//     display.drawVLine(64, 24, 16); // stem up

//     display.drawHLine(54, 24, 10); // bend left

//     display.drawVLine(54, 10, 14); // up on left

//     // Arrow tip pointing left

//     display.drawLine(54, 10, 60, 15);

//     display.drawLine(54, 10, 60, 5);

//     break;

//   case TURN_SLIGHT_RIGHT:

//     // Gentle diagonal right

//     display.drawVLine(64, 28, 12);

//     display.drawLine(64, 28, 70, 14);

//     display.drawVLine(70, 10, 4);

//     display.drawLine(70, 10, 65, 15);

//     display.drawLine(70, 10, 75, 15);

//     break;

//   case TURN_SLIGHT_LEFT:

//     // Gentle diagonal left

//     display.drawVLine(64, 28, 12);

//     display.drawLine(64, 28, 58, 14);

//     display.drawVLine(58, 10, 4);

//     display.drawLine(58, 10, 53, 15);

//     display.drawLine(58, 10, 63, 15);

//     break;

//   case TURN_U_TURN:

//     // U shape

//     display.drawVLine(60, 12, 18); // left stem

//     display.drawHLine(60, 30, 8); // bottom

//     display.drawVLine(68, 12, 18); // right stem

//     // Arrow pointing down on left stem

//     display.drawLine(60, 12, 55, 18);

//     display.drawLine(60, 12, 65, 18);

//     break;
//   }
// }

// // ── Draw small turn icon + distance in bottom half ───────────

// void drawTurnInfo(TurnDir turn, uint16_t dist)
// {

//   // ── Small turn arrow icon (left of distance) ─────────────

//   // Centred around x=48, y=50

//   switch (turn)
//   {

//   case TURN_STRAIGHT:

//     display.drawVLine(48, 44, 12);

//     display.drawLine(48, 44, 44, 49);

//     display.drawLine(48, 44, 52, 49);

//     break;

//   case TURN_RIGHT:

//     display.drawLine(44, 56, 44, 48);

//     display.drawLine(44, 48, 52, 48);

//     display.drawLine(52, 48, 48, 44);

//     display.drawLine(52, 48, 48, 52);

//     break;

//   case TURN_LEFT:

//     display.drawLine(52, 56, 52, 48);

//     display.drawLine(52, 48, 44, 48);

//     display.drawLine(44, 48, 48, 44);

//     display.drawLine(44, 48, 48, 52);

//     break;

//   case TURN_SLIGHT_RIGHT:

//     display.drawLine(44, 56, 50, 44);

//     display.drawLine(50, 44, 54, 50);

//     display.drawLine(50, 44, 44, 48);

//     break;

//   case TURN_SLIGHT_LEFT:

//     display.drawLine(52, 56, 46, 44);

//     display.drawLine(46, 44, 42, 50);

//     display.drawLine(46, 44, 52, 48);

//     break;

//   case TURN_U_TURN:

//     display.drawLine(46, 56, 46, 48);

//     display.drawHLine(46, 48, 6);

//     display.drawLine(52, 48, 52, 56);

//     display.drawLine(52, 56, 48, 52);

//     display.drawLine(52, 56, 56, 52);

//     break;
//   }

//   // ── Distance number ───────────────────────────────────────

//   display.setFont(u8g2_font_logisoso16_tf);

//   char distStr[6];

//   if (dist >= 1000)
//   {

//     snprintf(distStr, sizeof(distStr), "%d", dist / 1000);
//   }
//   else
//   {

//     snprintf(distStr, sizeof(distStr), "%d", dist);
//   }

//   display.drawStr(58, 62, distStr);

//   // ── Unit label ────────────────────────────────────────────

//   display.setFont(u8g2_font_5x7_tf);

//   if (dist >= 1000)
//   {

//     display.drawStr(58, 52, "km");
//   }
//   else
//   {

//     display.drawStr(58, 52, "m");
//   }
// }

// // ── Divider line across middle of circle ─────────────────────

// void drawDivider()
// {

//   // Horizontal line at y=36 clipped to circle width at that y

//   // Circle: cx=64, cy=32, r=28 → at y=36 (dy=4): x_span=sqrt(28²-4²)≈27

//   display.drawHLine(64 - 27, 36, 54);
// }

// // ── Street name at very bottom ────────────────────────────────

// void drawStreetName(const char *name)
// {

//   display.setFont(u8g2_font_4x6_tf);

//   uint8_t w = display.getStrWidth(name);

//   display.drawStr((128 - w) / 2, 64, name);
// }

// // ── Main draw ────────────────────────────────────────────────

// void drawNavScreen()
// {

//   display.clearBuffer();

//   drawCircleBorder();

//   drawRouteCurve(nav.turn);

//   drawDivider();

//   drawTurnInfo(nav.turn, nav.distanceMetres);

//   drawStreetName(nav.streetName);

//   display.sendBuffer();
// }

// // ── Call from loop() ─────────────────────────────────────────

// void handleNavigation()
// {

//   updateNavDemo();

//   drawNavScreen();
// }

// #endif // NAVIGATION_H

// ============================================================

//  navigation.h  --  Beeline-style nav UI for SH1106 128x64

//  Receives real nav data from web app over BLE (6-byte packet)

//  Falls back to random demo data if no BLE data received yet

//

//  BLE packet format (written by web app to NAV_CHAR):

//    byte 0   : TurnDir (0=straight 1=left 2=right 3=sl-left 4=sl-right 5=uturn)

//    byte 1-2 : distance metres uint16 big-endian

//    byte 3-5 : street name first 3 chars ASCII (0x00 padded)

//

//  Call handleNavigation() from loop()

//  Call updateNavFromBLE(data, len) from NavCallbacks::onWrite()

// ============================================================

// #ifndef NAVIGATION_H

// #define NAVIGATION_H

// #include <Arduino.h>

// #include <U8g2lib.h>

// extern U8G2_SH1106_128X64_NONAME_F_HW_I2C display;

// // Turn directions -- must match web app TURN_* constants

// enum TurnDir {

//   TURN_STRAIGHT     = 0,

//   TURN_LEFT         = 1,

//   TURN_RIGHT        = 2,

//   TURN_SLIGHT_LEFT  = 3,

//   TURN_SLIGHT_RIGHT = 4,

//   TURN_U_TURN       = 5

// };

// struct NavState {

//   TurnDir  turn;

//   uint16_t distanceMetres;

//   char     streetName[20];

//   bool     hasRealData;

//   uint8_t  rawBytes[6];

//   size_t   rawLen;

//   uint32_t packetCount;

// };

// NavState nav;

// unsigned long lastNavUpdate  = 0;

// unsigned long lastBLEReceive = 0;

// bool navDemoMode = true;

// const char* demoStreets[] = {

//   "MG Road","Ring Road","NH-48","Link Road","Station Rd"

// };

// // Called from NavCallbacks::onWrite() in main.cpp

// void updateNavFromBLE(const uint8_t* data, size_t len) {

//   if (len < 3) return;

//   uint8_t turnVal = data[0];

//   if (turnVal > 5) turnVal = 0;

//   nav.turn = (TurnDir)turnVal;

//   nav.distanceMetres = ((uint16_t)data[1] << 8) | data[2];

//   nav.rawLen = len > sizeof(nav.rawBytes) ? sizeof(nav.rawBytes) : len;

//   memcpy(nav.rawBytes, data, nav.rawLen);

//   nav.packetCount++;

//   if (len >= 6) {

//     nav.streetName[0] = (data[3] >= 0x20 && data[3] < 0x7F) ? (char)data[3] : ' ';

//     nav.streetName[1] = (data[4] >= 0x20 && data[4] < 0x7F) ? (char)data[4] : ' ';

//     nav.streetName[2] = (data[5] >= 0x20 && data[5] < 0x7F) ? (char)data[5] : 0;

//     nav.streetName[3] = 0;

//   } else {

//     nav.streetName[0] = 0;

//   }

//   nav.hasRealData = true;

//   navDemoMode     = false;

//   lastBLEReceive  = millis();

//   Serial.printf("Nav BLE: turn=%d dist=%dm street=%s\n",

//                 nav.turn, nav.distanceMetres, nav.streetName);

// }

// void updateNavDemo() {

//   unsigned long now = millis();

//   if (now - lastNavUpdate < 3000) return;

//   lastNavUpdate = now;

//   nav.turn           = (TurnDir)random(0, 6);

//   nav.distanceMetres = random(50, 900);

//   strncpy(nav.streetName, demoStreets[random(0, 5)], sizeof(nav.streetName) - 1);

//   nav.streetName[sizeof(nav.streetName) - 1] = 0;

// }

// void drawCircleBorder() {

//   display.drawCircle(64, 32, 30);

//   display.drawCircle(64, 32, 28);

// }

// void drawRouteCurve(TurnDir turn) {

//   switch (turn) {

//     case TURN_STRAIGHT:

//       display.drawVLine(64, 10, 30);

//       display.drawLine(64, 10, 59, 16);

//       display.drawLine(64, 10, 69, 16);

//       break;

//     case TURN_RIGHT:

//       display.drawVLine(64, 24, 16);

//       display.drawHLine(64, 24, 10);

//       display.drawVLine(74, 10, 14);

//       display.drawLine(74, 10, 68, 15);

//       display.drawLine(74, 10, 68, 5);

//       break;

//     case TURN_LEFT:

//       display.drawVLine(64, 24, 16);

//       display.drawHLine(54, 24, 10);

//       display.drawVLine(54, 10, 14);

//       display.drawLine(54, 10, 60, 15);

//       display.drawLine(54, 10, 60, 5);

//       break;

//     case TURN_SLIGHT_RIGHT:

//       display.drawVLine(64, 28, 12);

//       display.drawLine(64, 28, 70, 14);

//       display.drawVLine(70, 10, 4);

//       display.drawLine(70, 10, 65, 15);

//       display.drawLine(70, 10, 75, 15);

//       break;

//     case TURN_SLIGHT_LEFT:

//       display.drawVLine(64, 28, 12);

//       display.drawLine(64, 28, 58, 14);

//       display.drawVLine(58, 10, 4);

//       display.drawLine(58, 10, 53, 15);

//       display.drawLine(58, 10, 63, 15);

//       break;

//     case TURN_U_TURN:

//       display.drawVLine(60, 12, 18);

//       display.drawHLine(60, 30, 8);

//       display.drawVLine(68, 12, 18);

//       display.drawLine(60, 12, 55, 18);

//       display.drawLine(60, 12, 65, 18);

//       break;

//   }

// }

// void drawTurnInfo(TurnDir turn, uint16_t dist) {

//   switch (turn) {

//     case TURN_STRAIGHT:

//       display.drawVLine(48, 44, 12);

//       display.drawLine(48, 44, 44, 49);

//       display.drawLine(48, 44, 52, 49);

//       break;

//     case TURN_RIGHT:

//       display.drawLine(44, 56, 44, 48);

//       display.drawLine(44, 48, 52, 48);

//       display.drawLine(52, 48, 48, 44);

//       display.drawLine(52, 48, 48, 52);

//       break;

//     case TURN_LEFT:

//       display.drawLine(52, 56, 52, 48);

//       display.drawLine(52, 48, 44, 48);

//       display.drawLine(44, 48, 48, 44);

//       display.drawLine(44, 48, 48, 52);

//       break;

//     case TURN_SLIGHT_RIGHT:

//       display.drawLine(44, 56, 50, 44);

//       display.drawLine(50, 44, 54, 50);

//       display.drawLine(50, 44, 44, 48);

//       break;

//     case TURN_SLIGHT_LEFT:

//       display.drawLine(52, 56, 46, 44);

//       display.drawLine(46, 44, 42, 50);

//       display.drawLine(46, 44, 52, 48);

//       break;

//     case TURN_U_TURN:

//       display.drawLine(46, 56, 46, 48);

//       display.drawHLine(46, 48, 6);

//       display.drawLine(52, 48, 52, 56);

//       display.drawLine(52, 56, 48, 52);

//       display.drawLine(52, 56, 56, 52);

//       break;

//   }

//   display.setFont(u8g2_font_logisoso16_tf);

//   char distStr[8];

//   if (dist >= 1000) {

//     snprintf(distStr, sizeof(distStr), "%.1f", dist / 1000.0f);

//   } else {

//     snprintf(distStr, sizeof(distStr), "%d", dist);

//   }

//   display.drawStr(58, 62, distStr);

//   display.setFont(u8g2_font_5x7_tf);

//   display.drawStr(58, 52, dist >= 1000 ? "km" : "m");

// }

// void drawDivider() {

//   display.drawHLine(64 - 27, 36, 54);

// }

// void drawStreetName(const char* name) {

//   display.setFont(u8g2_font_4x6_tf);

//   uint8_t w = display.getStrWidth(name);

//   display.drawStr((128 - w) / 2, 64, name);

// }

// // Small dot top-right: solid=live fresh, outline=live stale, none=demo

// void drawPacketDebug() {

//   if (!nav.hasRealData) return;

//   char line[22];

//   snprintf(line, sizeof(line), "RX%lu %uB", (unsigned long)nav.packetCount, (unsigned)nav.rawLen);

//   display.setFont(u8g2_font_4x6_tf);

//   display.drawStr(0, 6, line);

// }

// void drawBLEIndicator() {

//   unsigned long age = millis() - lastBLEReceive;

//   if (!navDemoMode && age < 10000) {

//     display.drawDisc(123, 4, 3);

//   } else if (!navDemoMode) {

//     display.drawCircle(123, 4, 3);

//   }

// }

// void drawNavScreen() {

//   display.clearBuffer();

//   drawCircleBorder();

//   drawRouteCurve(nav.turn);

//   drawDivider();

//   drawTurnInfo(nav.turn, nav.distanceMetres);

//   drawStreetName(nav.streetName);

//   drawBLEIndicator();

//   display.sendBuffer();

// }

// void handleNavigation() {

//   if (navDemoMode) updateNavDemo();

//   drawNavScreen();

// }

// #endif // NAVIGATION_H

// ============================================================
//  navigation.h  --  Minimal Beeline-style nav UI
//  Layout: arrow left | dist+street right | progress bar bottom
//  BLE packet: byte0=turn, byte1-2=dist uint16 BE, byte3-5=street
// ============================================================
// #ifndef NAVIGATION_H
// #define NAVIGATION_H

// #include <Arduino.h>
// #include <U8g2lib.h>

// extern U8G2_SH1106_128X64_NONAME_F_HW_I2C display;

// // Must match web app TURN_* constants
// enum TurnDir
// {
//   TURN_STRAIGHT = 0,
//   TURN_LEFT = 1,
//   TURN_RIGHT = 2,
//   TURN_SLIGHT_LEFT = 3,
//   TURN_SLIGHT_RIGHT = 4,
//   TURN_U_TURN = 5
// };

// struct NavState
// {
//   TurnDir turn = TURN_STRAIGHT;
//   uint16_t distanceMetres = 200;
//   char streetName[20] = "MG Road";
//   bool hasRealData = false;
//   uint32_t packetCount = 0;
// };

// NavState nav;
// unsigned long lastNavUpdate = 0;
// unsigned long lastBLEReceive = 0;
// bool navDemoMode = true;

// const char *demoStreets[] = {
//     "MG Road", "Ring Road", "NH-48", "Link Road", "Station Rd"};

// // ── Called from NavCallbacks::onWrite() in main.cpp ──────────
// void updateNavFromBLE(const uint8_t *data, size_t len)
// {
//   if (len < 3)
//     return;
//   uint8_t t = data[0];
//   nav.turn = (TurnDir)(t > 5 ? 0 : t);
//   nav.distanceMetres = ((uint16_t)data[1] << 8) | data[2];
//   if (len >= 6)
//   {
//     for (int i = 0; i < 3; i++)
//       nav.streetName[i] = (data[3 + i] >= 0x20 && data[3 + i] < 0x7F) ? (char)data[3 + i] : ' ';
//     nav.streetName[3] = 0;
//   }
//   else
//   {
//     nav.streetName[0] = 0;
//   }
//   nav.hasRealData = true;
//   nav.packetCount++;
//   navDemoMode = false;
//   lastBLEReceive = millis();
//   Serial.printf("Nav BLE: turn=%d dist=%dm street=%s\n",
//                 nav.turn, nav.distanceMetres, nav.streetName);
// }

// // ── Demo cycling ──────────────────────────────────────────────
// void updateNavDemo()
// {
//   if (millis() - lastNavUpdate < 3000)
//     return;
//   lastNavUpdate = millis();
//   nav.turn = (TurnDir)random(0, 6);
//   nav.distanceMetres = random(50, 900);
//   strncpy(nav.streetName, demoStreets[random(0, 5)], sizeof(nav.streetName) - 1);
//   nav.streetName[sizeof(nav.streetName) - 1] = 0;
// }

// // ── Arrow drawing (solid filled, left half 0-45) ──────────────
// void drawNavArrow(TurnDir turn)
// {
//   // All arrows centred at cx=23, cy=36 (middle of left half below status bar)
//   const uint8_t cx = 23, cy = 36;

//   switch (turn)
//   {

//   case TURN_STRAIGHT:
//     // Shaft
//     display.drawBox(cx - 2, cy - 8, 5, 18);
//     // Head (triangle up)
//     for (int i = 0; i < 8; i++)
//       display.drawHLine(cx - i, cy - 8 - i, i * 2 + 1);
//     break;

//   case TURN_RIGHT:
//     // Vertical shaft up to mid
//     display.drawBox(cx - 2, cy - 2, 5, 14);
//     // Horizontal arm to right
//     display.drawBox(cx - 2, cy - 2, 14, 5);
//     // Arrow head pointing right
//     for (int i = 0; i < 7; i++)
//       display.drawVLine(cx + 7 + i, cy - i, i * 2 + 1);
//     break;

//   case TURN_LEFT:
//     // Vertical shaft up to mid
//     display.drawBox(cx - 2, cy - 2, 5, 14);
//     // Horizontal arm to left
//     display.drawBox(cx - 12, cy - 2, 14, 5);
//     // Arrow head pointing left
//     for (int i = 0; i < 7; i++)
//       display.drawVLine(cx - 7 - i, cy - i, i * 2 + 1);
//     break;

//   case TURN_SLIGHT_RIGHT:
//     display.drawBox(cx - 2, cy, 5, 10);
//     display.drawLine(cx, cy, cx + 10, cy - 10);
//     display.drawLine(cx + 10, cy - 10, cx + 4, cy - 10);
//     display.drawLine(cx + 10, cy - 10, cx + 10, cy - 4);
//     break;

//   case TURN_SLIGHT_LEFT:
//     display.drawBox(cx - 2, cy, 5, 10);
//     display.drawLine(cx, cy, cx - 10, cy - 10);
//     display.drawLine(cx - 10, cy - 10, cx - 4, cy - 10);
//     display.drawLine(cx - 10, cy - 10, cx - 10, cy - 4);
//     break;

//   case TURN_U_TURN:
//     // U shape
//     display.drawBox(cx - 8, cy - 4, 5, 16);
//     display.drawBox(cx + 3, cy - 4, 5, 16);
//     display.drawBox(cx - 8, cy + 9, 16, 5);
//     // Arrow tip pointing down on left stem
//     for (int i = 0; i < 5; i++)
//       display.drawHLine(cx - 8 - i, cy - 4 + i, 5 + i * 2);
//     break;
//   }
// }

// // ── Truncate street name ──────────────────────────────────────
// void truncateStreet(const char *src, char *dst, uint8_t maxChars)
// {
//   uint8_t len = strlen(src);
//   if (len <= maxChars)
//   {
//     strcpy(dst, src);
//     return;
//   }
//   strncpy(dst, src, maxChars - 1);
//   dst[maxChars - 1] = '\0';
//   strcat(dst, "~");
// }

// // ── Main nav screen ───────────────────────────────────────────
// void drawNavScreen()
// {
//   display.clearBuffer();

//   // ── Status bar (top 9px) ─────────────────────────────────
//   // BLE indicator dot (top-left)
//   if (nav.hasRealData && (millis() - lastBLEReceive < 10000))
//     display.drawDisc(4, 4, 3); // solid = fresh live data
//   else if (nav.hasRealData)
//     display.drawCircle(4, 4, 3); // outline = stale
//   else
//   {
//     display.setFont(u8g2_font_tom_thumb_4x6_tr);
//     display.drawStr(2, 7, "DEMO");
//   }

//   // Packet counter (only when live)
//   if (nav.hasRealData)
//   {
//     display.setFont(u8g2_font_tom_thumb_4x6_tr);
//     char pkt[8];
//     snprintf(pkt, sizeof(pkt), "#%lu", (unsigned long)nav.packetCount);
//     display.drawStr(10, 7, pkt);
//   }

//   display.drawHLine(0, 9, 128);

//   // ── Vertical separator ────────────────────────────────────
//   display.drawVLine(46, 10, 54);

//   // ── Left half: arrow ─────────────────────────────────────
//   drawNavArrow(nav.turn);

//   // ── Right half: distance ──────────────────────────────────
//   char distStr[8];
//   if (nav.distanceMetres >= 1000)
//     snprintf(distStr, sizeof(distStr), "%.1fkm", nav.distanceMetres / 1000.0f);
//   else
//     snprintf(distStr, sizeof(distStr), "%dm", nav.distanceMetres);

//   display.setFont(u8g2_font_logisoso16_tr);
//   uint8_t dw = display.getStrWidth(distStr);
//   display.drawStr(50 + (78 - dw) / 2, 30, distStr);

//   display.setFont(u8g2_font_tom_thumb_4x6_tr);
//   display.drawStr(50 + (78 - display.getStrWidth("in")) / 2, 40, "in");

//   char streetBuf[12];
//   truncateStreet(nav.streetName, streetBuf, 10);
//   uint8_t sw = display.getStrWidth(streetBuf);
//   display.drawStr(50 + (78 - sw) / 2, 50, streetBuf);

//   // ── Progress bar (bottom 4px) ─────────────────────────────
//   display.drawHLine(0, 56, 128);
//   uint16_t clamp = nav.distanceMetres > 500 ? 500 : nav.distanceMetres;
//   uint8_t progW = (uint8_t)(128UL * (500 - clamp) / 500);
//   if (progW > 0)
//     display.drawBox(0, 57, progW, 4);

//   display.sendBuffer();
// }

// // ── Call from loop() ──────────────────────────────────────────
// void handleNavigation()
// {
//   if (navDemoMode)
//     updateNavDemo();
//   drawNavScreen();
// }

// #endif // NAVIGATION_H

// ============================================================
//  navigation.h  --  Minimal Beeline-style nav UI
//  Layout: 50/50 split — arrow left | dist right
//          Top-right: ESP32 battery
//          Bottom: journey progress bar
// ============================================================
#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <Arduino.h>
#include <U8g2lib.h>

extern U8G2_SH1106_128X64_NONAME_F_HW_I2C display;

// Must match web app TURN_* constants
enum TurnDir
{
  TURN_STRAIGHT = 0,
  TURN_LEFT = 1,
  TURN_RIGHT = 2,
  TURN_SLIGHT_LEFT = 3,
  TURN_SLIGHT_RIGHT = 4,
  TURN_U_TURN = 5
};

struct NavState
{
  TurnDir turn = TURN_STRAIGHT;
  uint16_t distanceMetres = 200;   // distance to next turn
  uint16_t totalMetres = 5000;     // total journey distance
  uint16_t remainingMetres = 4800; // remaining journey distance
  char streetName[20] = "MG Road";
  bool hasRealData = false;
  uint32_t packetCount = 0;
};

NavState nav;
unsigned long lastNavUpdate = 0;
unsigned long lastBLEReceive = 0;
bool navDemoMode = true;

// Battery (read from ADC in main.cpp or set directly)
uint8_t navBatPercent = 100;

const char *demoStreets[] = {
    "MG Road", "Ring Road", "NH-48", "Link Road", "Station Rd"};

// ── Battery ADC read (call from setup + loop periodically) ────
// GPIO34, 100k+100k divider: 4.2V→ADC~2607, 3.0V→ADC~1862
uint8_t readNavBattery()
{
  uint32_t raw = analogRead(34);
  const uint32_t EMPTY = 1862, FULL = 2607;
  if (raw <= EMPTY)
    return 0;
  if (raw >= FULL)
    return 100;
  return (uint8_t)((raw - EMPTY) * 100UL / (FULL - EMPTY));
}

// ── BLE packet parser ─────────────────────────────────────────
// byte 0   : TurnDir
// byte 1-2 : distance to turn uint16 BE
// byte 3-4 : total journey metres uint16 BE      (new)
// byte 5-6 : remaining journey metres uint16 BE  (new)
// byte 7-9 : street name first 3 chars ASCII
void updateNavFromBLE(const uint8_t *data, size_t len)
{
  if (len < 3)
    return;

  uint8_t t = data[0];
  nav.turn = (TurnDir)(t > 5 ? 0 : t);
  nav.distanceMetres = ((uint16_t)data[1] << 8) | data[2];

  if (len >= 5)
    nav.totalMetres = ((uint16_t)data[3] << 8) | data[4];
  if (len >= 7)
    nav.remainingMetres = ((uint16_t)data[5] << 8) | data[6];

  // Street name (3 chars from byte 7 or byte 3 for old 6-byte format)
  uint8_t nameOffset = (len >= 8) ? 7 : 3;
  if (len > nameOffset)
  {
    for (int i = 0; i < 3 && (nameOffset + i) < len; i++)
    {
      uint8_t ch = data[nameOffset + i];
      nav.streetName[i] = (ch >= 0x20 && ch < 0x7F) ? (char)ch : ' ';
    }
    nav.streetName[3] = 0;
  }
  else
  {
    nav.streetName[0] = 0;
  }

  nav.hasRealData = true;
  nav.packetCount++;
  navDemoMode = false;
  lastBLEReceive = millis();

  Serial.printf("Nav BLE: turn=%d dist=%dm total=%dm remain=%dm street=%s\n",
                nav.turn, nav.distanceMetres, nav.totalMetres, nav.remainingMetres, nav.streetName);
}

// ── Demo data ─────────────────────────────────────────────────
void updateNavDemo()
{
  if (millis() - lastNavUpdate < 3000)
    return;
  lastNavUpdate = millis();
  nav.turn = (TurnDir)random(0, 6);
  nav.distanceMetres = random(50, 900);
  nav.totalMetres = 5000;
  nav.remainingMetres = random(500, 4900);
  strncpy(nav.streetName, demoStreets[random(0, 5)], sizeof(nav.streetName) - 1);
  nav.streetName[sizeof(nav.streetName) - 1] = 0;
}

// ── Arrow drawing — fills left 50% (0–63), content area y=10–55
//    Centre: cx=32, cy=32
void drawNavArrow(TurnDir turn)
{
  const uint8_t cx = 32, cy = 33;
  const uint8_t S = 14; // scale

  switch (turn)
  {

  case TURN_STRAIGHT:
    display.drawBox(cx - 3, cy - S + 2, 7, S * 2 - 4); // shaft
    for (uint8_t i = 0; i < S; i++)                    // triangle head up
      display.drawHLine(cx - i, cy - S + 2 - i, i * 2 + 1);
    break;

  case TURN_RIGHT:
    display.drawBox(cx - 3, cy, 7, S);         // shaft down
    display.drawBox(cx - 3, cy - 3, S + 4, 7); // arm right
    for (uint8_t i = 0; i < S - 2; i++)        // head right
      display.drawVLine(cx + S + 1 + i, cy - 3 - i, 7 + i * 2);
    break;

  case TURN_LEFT:
    display.drawBox(cx - 3, cy, 7, S);             // shaft down
    display.drawBox(cx - S - 1, cy - 3, S + 4, 7); // arm left
    for (uint8_t i = 0; i < S - 2; i++)            // head left
      display.drawVLine(cx - S - i, cy - 3 - i, 7 + i * 2);
    break;

  case TURN_SLIGHT_RIGHT:
    display.drawBox(cx - 3, cy + 2, 7, S - 2);
    display.drawLine(cx, cy + 2, cx + S, cy - S + 2);
    display.drawLine(cx + S, cy - S + 2, cx + S - 6, cy - S + 2);
    display.drawLine(cx + S, cy - S + 2, cx + S, cy - S + 8);
    display.drawLine(cx + S - 1, cy - S + 3, cx + S + 4, cy - S + 3);
    display.drawLine(cx + S - 1, cy - S + 3, cx + S - 1, cy - S + 9);
    break;

  case TURN_SLIGHT_LEFT:
    display.drawBox(cx - 3, cy + 2, 7, S - 2);
    display.drawLine(cx, cy + 2, cx - S, cy - S + 2);
    display.drawLine(cx - S, cy - S + 2, cx - S + 6, cy - S + 2);
    display.drawLine(cx - S, cy - S + 2, cx - S, cy - S + 8);
    display.drawLine(cx - S + 1, cy - S + 3, cx - S - 4, cy - S + 3);
    display.drawLine(cx - S + 1, cy - S + 3, cx - S + 1, cy - S + 9);
    break;

  case TURN_U_TURN:
    display.drawBox(cx - 10, cy - S + 4, 6, S + 4); // left stem
    display.drawBox(cx + 4, cy - S + 4, 6, S + 4);  // right stem
    display.drawBox(cx - 10, cy + 5, 20, 6);        // bottom U
    // arrow tip down on left stem
    for (uint8_t i = 0; i < 6; i++)
      display.drawHLine(cx - 10 - i, cy - S + 4 + i, 6 + i * 2);
    break;
  }
}

// ── Truncate street name ──────────────────────────────────────
void truncateStreet(const char *src, char *dst, uint8_t maxChars)
{
  uint8_t len = strlen(src);
  if (len <= maxChars)
  {
    strcpy(dst, src);
    return;
  }
  strncpy(dst, src, maxChars - 1);
  dst[maxChars - 1] = '\0';
  strcat(dst, "~");
}

// ── Battery bar (top-right) ───────────────────────────────────
// Draws at x=96, y=1 — 28px wide, 7px tall
void drawBatteryBar(uint8_t pct)
{
  const uint8_t bx = 96, by = 1, bw = 28, bh = 7;
  display.drawFrame(bx, by, bw, bh);
  display.drawBox(bx + bw, by + 2, 2, 3); // nub
  uint8_t fill = (uint8_t)((pct * (uint32_t)(bw - 2)) / 100);
  if (fill > 0)
    display.drawBox(bx + 1, by + 1, fill, bh - 2);
  // % label
  display.setFont(u8g2_font_tom_thumb_4x6_tr);
  char buf[5];
  snprintf(buf, sizeof(buf), "%d%%", pct);
  display.drawStr(bx - display.getStrWidth(buf) - 2, by + 6, buf);
}

// ── Main nav screen ───────────────────────────────────────────
void drawNavScreen()
{
  display.clearBuffer();

  // ── Status bar (top 9px) ─────────────────────────────────
  // BLE/demo indicator (top-left)
  display.setFont(u8g2_font_tom_thumb_4x6_tr);
  if (!nav.hasRealData)
  {
    display.drawStr(1, 7, "DEMO");
  }
  else
  {
    unsigned long age = millis() - lastBLEReceive;
    if (age < 10000)
      display.drawDisc(4, 4, 3);
    else
      display.drawCircle(4, 4, 3);
  }

  // Battery top-right
  drawBatteryBar(navBatPercent);

  // Divider
  display.drawHLine(0, 9, 128);

  // ── Content area: y=10 to y=55, split 50/50 at x=64 ─────
  display.drawVLine(64, 10, 46);

  // ── Left 50%: arrow (cx=32, content y=10–55) ─────────────
  drawNavArrow(nav.turn);

  // ── Right 50%: distance + street (x=68..127 = 60px wide) ─
  char distStr[8];
  if (nav.distanceMetres >= 1000)
    snprintf(distStr, sizeof(distStr), "%.1fkm", nav.distanceMetres / 1000.0f);
  else
    snprintf(distStr, sizeof(distStr), "%dm", nav.distanceMetres);

  // Distance — large font, vertically centred in right half
  display.setFont(u8g2_font_logisoso16_tr);
  uint8_t dw = display.getStrWidth(distStr);
  display.drawStr(66 + (60 - dw) / 2, 32, distStr);

  // "in" tiny label
  display.setFont(u8g2_font_tom_thumb_4x6_tr);
  display.drawStr(66 + (60 - display.getStrWidth("in")) / 2, 41, "in");

  // Street name
  char streetBuf[10];
  truncateStreet(nav.streetName, streetBuf, 8);
  uint8_t sw = display.getStrWidth(streetBuf);
  display.drawStr(66 + (60 - sw) / 2, 51, streetBuf);

  // ── Bottom: journey progress bar (y=56–63) ───────────────
  display.drawHLine(0, 56, 128);
  if (nav.totalMetres > 0)
  {
    uint32_t done = (nav.totalMetres > nav.remainingMetres)
                        ? (nav.totalMetres - nav.remainingMetres)
                        : 0;
    uint8_t progW = (uint8_t)(128UL * done / nav.totalMetres);
    if (progW > 0)
      display.drawBox(0, 57, progW, 6);
  }

  display.sendBuffer();
}

// ── Call from loop() ──────────────────────────────────────────
void handleNavigation()
{
  if (navDemoMode)
    updateNavDemo();
  drawNavScreen();
}

#endif // NAVIGATION_H