// ============================================================
//  main.cpp  –  ESP32 BLE HID (NimBLE-Arduino v2.x)
//  Roles : HID Peripheral (media + call) + GATT Server (nav)
//
//  platformio.ini lib_deps:
//    h2zero/NimBLE-Arduino @ ^2.5.0
// ============================================================
#include <Arduino.h>
#include <NimBLEDevice.h>
#include <NimBLEServer.h>
#include <NimBLEHIDDevice.h>
#include <NimBLECharacteristic.h>

#include "media_control.h"
#include "call_control.h"

// ── HID Report IDs ───────────────────────────────────────────
#define REPORT_ID_CONSUMER   0x01
#define REPORT_ID_TELEPHONY  0x02

// ── Beeline Navigation GATT UUIDs ────────────────────────────
#define NAV_SERVICE_UUID  "12345678-1234-1234-1234-123456789abc"
#define NAV_CHAR_UUID     "abcdefab-cdef-cdef-cdef-abcdefabcdef"

// ── Globals ──────────────────────────────────────────────────
NimBLECharacteristic* mediaInputChar = nullptr;
NimBLECharacteristic* callInputChar  = nullptr;
NimBLECharacteristic* navChar        = nullptr;

bool deviceConnected = false;
bool wasConnected    = false;

// ── HID Report Descriptor ────────────────────────────────────
static const uint8_t hidReportDescriptor[] = {

  // Consumer Control – array style, 2-byte usage ID
  0x05, 0x0C,                   // Usage Page (Consumer)
  0x09, 0x01,                   // Usage (Consumer Control)
  0xA1, 0x01,                   // Collection (Application)
    0x85, REPORT_ID_CONSUMER,   //   Report ID 1
    0x15, 0x00,                 //   Logical Minimum (0)
    0x26, 0xFF, 0x03,           //   Logical Maximum (1023)
    0x19, 0x00,                 //   Usage Minimum (0)
    0x2A, 0xFF, 0x03,           //   Usage Maximum (1023)
    0x75, 0x10,                 //   Report Size (16 bit)
    0x95, 0x01,                 //   Report Count (1)
    0x81, 0x00,                 //   Input (Data, Array, Absolute)
  0xC0,                         // End Collection

  // Telephony Device – array style, 2-byte usage ID
  0x05, 0x0B,                   // Usage Page (Telephony)
  0x09, 0x01,                   // Usage (Phone)
  0xA1, 0x01,                   // Collection (Application)
    0x85, REPORT_ID_TELEPHONY,  //   Report ID 2
    0x15, 0x00,                 //   Logical Minimum (0)
    0x26, 0xFF, 0x03,           //   Logical Maximum (1023)
    0x19, 0x00,                 //   Usage Minimum (0)
    0x2A, 0xFF, 0x03,           //   Usage Maximum (1023)
    0x75, 0x10,                 //   Report Size (16 bit)
    0x95, 0x01,                 //   Report Count (1)
    0x81, 0x00,                 //   Input (Data, Array, Absolute)
  0xC0                          // End Collection
};

// ── Server Callbacks ──────────────────────────────────────────
class ServerCallbacks : public NimBLEServerCallbacks {
  void onConnect(NimBLEServer* pSrv, NimBLEConnInfo& connInfo) override {
    deviceConnected = true;
    Serial.println("BLE Connected");
    NimBLEDevice::startAdvertising();
  }
  void onDisconnect(NimBLEServer* pSrv, NimBLEConnInfo& connInfo, int reason) override {
    deviceConnected = false;
    Serial.println("BLE Disconnected - restarting advertising");
    NimBLEDevice::startAdvertising();
  }
};

// ── Beeline Nav Write Callback ────────────────────────────────
class NavCallbacks : public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic* pChar, NimBLEConnInfo& connInfo) override {
    std::string val = pChar->getValue();
    if (val.empty()) return;
    Serial.printf("Nav data (%d bytes): ", val.length());
    for (char c : val) Serial.printf("%02X ", (uint8_t)c);
    Serial.println();
  }
};

// ── Setup ─────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n=== ESP32 NimBLE HID + Nav ===");

  setupMediaPins();
  setupCallPins();

  NimBLEDevice::init("ESP32 Bike Control");
  NimBLEDevice::setPower(3);

  NimBLEServer* pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  // ── HID Device ───────────────────────────────────────────
  NimBLEHIDDevice* hid = new NimBLEHIDDevice(pServer);
  hid->setManufacturer("ESP32");
  hid->setPnp(0x02, 0x045E, 0x0000, 0x0110);
  hid->setHidInfo(0x00, 0x01);
  hid->setReportMap((uint8_t*)hidReportDescriptor, sizeof(hidReportDescriptor));

  mediaInputChar = hid->getInputReport(REPORT_ID_CONSUMER);
  callInputChar  = hid->getInputReport(REPORT_ID_TELEPHONY);

  NimBLEDevice::setSecurityAuth(BLE_SM_PAIR_AUTHREQ_BOND);

  // ── Beeline Navigation GATT Service ──────────────────────
  NimBLEService* navSvc = pServer->createService(NAV_SERVICE_UUID);
  navChar = navSvc->createCharacteristic(
    NAV_CHAR_UUID,
    NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR | NIMBLE_PROPERTY::NOTIFY
  );
  navChar->setCallbacks(new NavCallbacks());

  pServer->start();

  // ── Advertising (Android 15 compatible) ──────────────────
  NimBLEAdvertising* pAdv = NimBLEDevice::getAdvertising();
  pAdv->setAppearance(HID_KEYBOARD);
  pAdv->addServiceUUID(NAV_SERVICE_UUID);

  NimBLEAdvertisementData scanResp;
  scanResp.setName("ESP32 Bike Control");
  pAdv->setScanResponseData(scanResp);

  pAdv->enableScanResponse(true);
  NimBLEDevice::startAdvertising();

  Serial.println("Advertising... waiting for connection");
}

// ── Loop ──────────────────────────────────────────────────────
void loop() {
  if (!deviceConnected) {
    if (wasConnected) {
      wasConnected = false;
      Serial.println("Waiting for BLE connection...");
    }
    delay(500);
    return;
  }

  if (!wasConnected) {
    wasConnected = true;
    Serial.println("BLE Connection Established");
  }

  handleMediaControl();
  handleCallControl();

  delay(20);
}