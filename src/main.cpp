// // =========================

// //  main.cpp  --  ESP32 NimBLE HID + OLED + Beeline Nav UI

// // ============================================================

// #include <Arduino.h>

// #include <NimBLEDevice.h>

// #include <NimBLEServer.h>

// #include <NimBLEHIDDevice.h>

// #include <NimBLECharacteristic.h>

// #include "media_control.h"

// #include "call_control.h"

// #include "display.h"

// #include "navigation.h"

// // HID Report ID

// #define REPORT_ID_CONSUMER 0x01

// // Beeline Navigation GATT UUIDs

// #define NAV_SERVICE_UUID "12345678-1234-1234-1234-123456789abc"

// #define NAV_CHAR_UUID "abcdefab-cdef-cdef-cdef-abcdefabcdef"

// // Globals

// NimBLECharacteristic *mediaInputChar = nullptr;

// NimBLECharacteristic *navChar = nullptr;

// bool deviceConnected = false;

// bool wasConnected = false;

// // Screen toggle: long press Play/Pause (pin 26) > 800ms switches screen

// enum Screen
// {
//     SCREEN_MEDIA,
//     SCREEN_NAV
// };

// Screen currentScreen = SCREEN_MEDIA;

// unsigned long ppHeldAt = 0;

// bool ppWasHeld = false;

// // HID Report Descriptor

// //

// //  Single Consumer report (ID 1) -- 1 byte, 7 bits used:

// //

// //    bit 0 = Play/Pause   (0xCD)  -- media play/pause (pin 26)

// //    bit 1 = Next Track   (0xB5)  -- media next       (pin 25)

// //    bit 2 = Prev Track   (0xB6)  -- media prev       (pin 27)

// //    bit 3 = Stop         (0xB7)  -- media stop

// //    bit 4 = Volume Up    (0xE9)

// //    bit 5 = Volume Down  (0xEA)  -- decline call hold 2s (pin 33)

// //    bit 6 = Play         (0xB0)  -- answer/end call held 200ms (pin 32)

// //                                    Android routes 0xB0 to call manager

// //                                    independently from Play/Pause (0xCD)

// //    bit 7 = padding

// static const uint8_t hidReportDescriptor[] = {

//     0x05,
//     0x0C,

//     0x09,
//     0x01,

//     0xA1,
//     0x01,

//     0x85,
//     0x01,

//     0x15,
//     0x00,

//     0x25,
//     0x01,

//     0x75,
//     0x01,

//     0x95,
//     0x07,

//     0x09,
//     0xCD,

//     0x09,
//     0xB5,

//     0x09,
//     0xB6,

//     0x09,
//     0xB7,

//     0x09,
//     0xE9,

//     0x09,
//     0xEA,

//     0x09,
//     0xB0,

//     0x81,
//     0x02,

//     0x95,
//     0x01,

//     0x75,
//     0x01,

//     0x81,
//     0x03,

//     0xC0,

// };

// // Server Callbacks -- handle BLE connection/disconnection events

// class ServerCallbacks : public NimBLEServerCallbacks

// {

//     void onConnect(NimBLEServer *pSrv, NimBLEConnInfo &connInfo) override

//     {

//         deviceConnected = true;

//         Serial.println("BLE Connected");

//         NimBLEDevice::startAdvertising();
//     }

//     void onDisconnect(NimBLEServer *pSrv, NimBLEConnInfo &connInfo, int reason) override

//     {

//         deviceConnected = false;

//         Serial.println("BLE Disconnected - restarting advertising");

//         NimBLEDevice::startAdvertising();
//     }
// };

// // Nav Write Callback

// class NavCallbacks : public NimBLECharacteristicCallbacks

// {

//     void onWrite(NimBLECharacteristic *pChar, NimBLEConnInfo &connInfo) override

//     {

//         std::string val = pChar->getValue();

//         if (val.empty())
//             return;

//         Serial.printf("Nav data (%d bytes): ", val.length());

//         for (char c : val)

//             Serial.printf("%02X ", (uint8_t)c);

//         Serial.println();
//     }
// };

// // Screen toggle handler

// void handleScreenToggle()

// {

//     bool ppPressed = digitalRead(PlayPause_Button);

//     if (ppPressed == HIGH)

//     {

//         if (ppHeldAt == 0)
//             ppHeldAt = millis();

//         if (!ppWasHeld && (millis() - ppHeldAt > 800))

//         {

//             currentScreen = (currentScreen == SCREEN_MEDIA) ? SCREEN_NAV : SCREEN_MEDIA;

//             Serial.println(currentScreen == SCREEN_NAV ? "Screen: NAV" : "Screen: MEDIA");

//             ppWasHeld = true;
//         }
//     }

//     else

//     {

//         ppHeldAt = 0;

//         ppWasHeld = false;
//     }
// }

// // Setup

// void setup()

// {

//     Serial.begin(115200);

//     delay(500);

//     Serial.println("\n=== ESP32 NimBLE HID + OLED Nav ===");

//     setupDisplay();

//     setupMediaPins();

//     setupCallPins();

//     NimBLEDevice::init("ESP32 Bike Control");

//     NimBLEDevice::setPower(9);

//     // No bonding -- pairs with any device without bond conflict

//     NimBLEDevice::setSecurityAuth(false, false, false);

//     NimBLEServer *pServer = NimBLEDevice::createServer();

//     pServer->setCallbacks(new ServerCallbacks());

//     NimBLEHIDDevice *hid = new NimBLEHIDDevice(pServer);

//     hid->setManufacturer("ESP32");

//     hid->setPnp(0x02, 0x045E, 0x0000, 0x0110);

//     hid->setHidInfo(0x00, 0x01);

//     hid->setReportMap((uint8_t *)hidReportDescriptor, sizeof(hidReportDescriptor));

//     mediaInputChar = hid->getInputReport(REPORT_ID_CONSUMER);

//     NimBLEService *navSvc = pServer->createService(NAV_SERVICE_UUID);

//     navChar = navSvc->createCharacteristic(

//         NAV_CHAR_UUID,

//         NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR | NIMBLE_PROPERTY::NOTIFY);

//     navChar->setCallbacks(new NavCallbacks());

//     pServer->start();

//     NimBLEAdvertising *pAdv = NimBLEDevice::getAdvertising();

//     pAdv->addServiceUUID(hid->getHidService()->getUUID());

//     pAdv->addServiceUUID(NAV_SERVICE_UUID);

//     NimBLEAdvertisementData scanResp;

//     scanResp.setAppearance(HID_KEYBOARD);

//     scanResp.setName("ESP32 Bike Control");

//     pAdv->setScanResponseData(scanResp);

//     pAdv->enableScanResponse(true);

//     NimBLEDevice::startAdvertising();

//     Serial.println("Advertising... waiting for connection");
// }

// // Loop

// void loop()

// {

//     handleScreenToggle();

//     if (currentScreen == SCREEN_NAV)

//         handleNavigation();

//     else

//         handleDisplay(deviceConnected);

//     if (!deviceConnected)

//     {

//         if (wasConnected)

//         {

//             wasConnected = false;

//             Serial.println("Waiting for BLE connection...");
//         }

//         delay(20);

//         return;
//     }

//     if (!wasConnected)

//     {

//         wasConnected = true;

//         Serial.println("BLE Connection Established");
//     }

//     handleMediaControl();

//     handleCallControl();

//     delay(20);
// }
#include <Arduino.h>

#include <NimBLEDevice.h>

#include <NimBLEServer.h>

#include <NimBLEHIDDevice.h>

#include <NimBLECharacteristic.h>

 

#include "media_control.h"

#include "call_control.h"

#include "display.h"

#include "navigation.h"

 

// HID Report ID

#define REPORT_ID_CONSUMER 0x01

 

// Beeline Navigation GATT UUIDs

#define NAV_SERVICE_UUID "12345678-1234-1234-1234-123456789abc"

#define NAV_CHAR_UUID    "abcdefab-cdef-cdef-cdef-abcdefabcdef"

 

// Globals

NimBLECharacteristic *mediaInputChar = nullptr;

NimBLECharacteristic *navChar        = nullptr;

 

bool deviceConnected = false;

bool wasConnected    = false;

 

// Screen toggle: long press Play/Pause (pin 26) > 800ms switches screen

enum Screen { SCREEN_MEDIA, SCREEN_NAV };

Screen        currentScreen = SCREEN_MEDIA;

unsigned long ppHeldAt      = 0;

bool          ppWasHeld     = false;

 

// HID Report Descriptor

//

//  Single Consumer report (ID 1) -- 1 byte, 7 bits used:

//

//    bit 0 = Play/Pause   (0xCD)  -- media play/pause (pin 26)

//    bit 1 = Next Track   (0xB5)  -- media next       (pin 25)

//    bit 2 = Prev Track   (0xB6)  -- media prev       (pin 27)

//    bit 3 = Stop         (0xB7)  -- media stop

//    bit 4 = Volume Up    (0xE9)

//    bit 5 = Volume Down  (0xEA)  -- decline call hold 2s (pin 33)

//    bit 6 = Play         (0xB0)  -- answer/end call held 200ms (pin 32)

//                                    Android routes 0xB0 to call manager

//                                    independently from Play/Pause (0xCD)

//    bit 7 = padding

static const uint8_t hidReportDescriptor[] = {

    0x05, 0x0C,

    0x09, 0x01,

    0xA1, 0x01,

    0x85, 0x01,

    0x15, 0x00,

    0x25, 0x01,

    0x75, 0x01,

    0x95, 0x07,

    0x09, 0xCD,

    0x09, 0xB5,

    0x09, 0xB6,

    0x09, 0xB7,

    0x09, 0xE9,

    0x09, 0xEA,

    0x09, 0xB0,

    0x81, 0x02,

    0x95, 0x01,

    0x75, 0x01,

    0x81, 0x03,

    0xC0,

};

 

// Server Callbacks

class ServerCallbacks : public NimBLEServerCallbacks

{

    void onConnect(NimBLEServer *pSrv, NimBLEConnInfo &connInfo) override

    {

        deviceConnected = true;

        Serial.println("BLE Connected");

        NimBLEDevice::startAdvertising();

    }

    void onDisconnect(NimBLEServer *pSrv, NimBLEConnInfo &connInfo, int reason) override

    {

        deviceConnected = false;

        Serial.println("BLE Disconnected - restarting advertising");

        NimBLEDevice::startAdvertising();

    }

};

 

// Nav Write Callback

class NavCallbacks : public NimBLECharacteristicCallbacks

{

    void onWrite(NimBLECharacteristic *pChar, NimBLEConnInfo &connInfo) override

    {

        std::string val = pChar->getValue();

        if (val.empty()) return;

        // Forward raw bytes to navigation.h parser

        updateNavFromBLE((const uint8_t*)val.data(), val.length());

    }

};

 

// Screen toggle handler

void handleScreenToggle()

{

    bool ppPressed = digitalRead(PlayPause_Button);

 

    if (ppPressed == HIGH)

    {

        if (ppHeldAt == 0) ppHeldAt = millis();

        if (!ppWasHeld && (millis() - ppHeldAt > 800))

        {

            currentScreen = (currentScreen == SCREEN_MEDIA) ? SCREEN_NAV : SCREEN_MEDIA;

            Serial.println(currentScreen == SCREEN_NAV ? "Screen: NAV" : "Screen: MEDIA");

            ppWasHeld = true;

        }

    }

    else

    {

        ppHeldAt  = 0;

        ppWasHeld = false;

    }

}

 

// Setup

void setup()

{

    Serial.begin(115200);

    delay(500);

    Serial.println("\n=== ESP32 NimBLE HID + OLED Nav ===");

 

    setupDisplay();

    setupMediaPins();

    setupCallPins();

 

    NimBLEDevice::init("ESP32 Bike Control");

    NimBLEDevice::setPower(9);

 

    // No bonding -- pairs with any device without bond conflict

    NimBLEDevice::setSecurityAuth(false, false, false);

 

    NimBLEServer *pServer = NimBLEDevice::createServer();

    pServer->setCallbacks(new ServerCallbacks());

 

    NimBLEHIDDevice *hid = new NimBLEHIDDevice(pServer);

    hid->setManufacturer("ESP32");

    hid->setPnp(0x02, 0x045E, 0x0000, 0x0110);

    hid->setHidInfo(0x00, 0x01);

    hid->setReportMap((uint8_t *)hidReportDescriptor, sizeof(hidReportDescriptor));

 

    mediaInputChar = hid->getInputReport(REPORT_ID_CONSUMER);

 

    NimBLEService *navSvc = pServer->createService(NAV_SERVICE_UUID);

    navChar = navSvc->createCharacteristic(

        NAV_CHAR_UUID,

        NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR | NIMBLE_PROPERTY::NOTIFY);

    navChar->setCallbacks(new NavCallbacks());

 

    pServer->start();

 

    NimBLEAdvertising *pAdv = NimBLEDevice::getAdvertising();

    pAdv->addServiceUUID(hid->getHidService()->getUUID());

    pAdv->addServiceUUID(NAV_SERVICE_UUID);

 

    NimBLEAdvertisementData scanResp;

    scanResp.setAppearance(HID_KEYBOARD);

    scanResp.setName("ESP32 Bike Control");

    pAdv->setScanResponseData(scanResp);

 

    pAdv->enableScanResponse(true);

    NimBLEDevice::startAdvertising();

 

    Serial.println("Advertising... waiting for connection");

}

 

// Loop

void loop()

{

    handleScreenToggle();

 

    if (currentScreen == SCREEN_NAV)

        handleNavigation();

    else

        handleDisplay(deviceConnected);

 

    if (!deviceConnected)

    {

        if (wasConnected)

        {

            wasConnected = false;

            Serial.println("Waiting for BLE connection...");

        }

        delay(20);

        return;

    }

 

    if (!wasConnected)

    {

        wasConnected = true;

        Serial.println("BLE Connection Established");

    }

 

    handleMediaControl();

    handleCallControl();

 

    delay(20);

}