// ============================================================

//  media_control.h  --  NimBLE HID Consumer Media Keys

//  Pin 27 = Previous | Pin 26 = Play/Pause | Pin 25 = Next

//  Logic: INPUT_PULLDOWN, rising-edge (LOW->HIGH) detection

// ============================================================

#ifndef MEDIA_CONTROL_H

#define MEDIA_CONTROL_H

 

#include <Arduino.h>

#include <NimBLECharacteristic.h>

 

// Pin Definitions

const int Prev_Button      = 27;

const int PlayPause_Button = 26;

const int Next_Button      = 25;

 

// Debounce States

static bool prevState      = LOW;

static bool playPauseState = LOW;

static bool nextState      = LOW;

 

// Bitmask positions -- must match descriptor order in main.cpp

// bit 0 = Play/Pause  (0xCD)  -- MEDIA ONLY (pin 26)

// bit 1 = Next Track  (0xB5)  -- MEDIA (pin 25)

// bit 2 = Prev Track  (0xB6)  -- MEDIA (pin 27)

// bit 3 = Stop        (0xB7)

// bit 4 = Volume Up   (0xE9)

// bit 5 = Volume Down (0xEA)  -- decline call (pin 33, call_control.h)

// bit 6 = Play        (0xB0)  -- answer/end call (pin 32, call_control.h)

// bit 7 = padding

#define BIT_PLAY_PAUSE   (1 << 0)

#define BIT_NEXT_TRACK   (1 << 1)

#define BIT_PREV_TRACK   (1 << 2)

#define BIT_STOP         (1 << 3)

#define BIT_VOLUME_UP    (1 << 4)

#define BIT_VOLUME_DOWN  (1 << 5)

#define BIT_PLAY         (1 << 6)

 

// Extern -- defined in main.cpp

extern NimBLECharacteristic *mediaInputChar;

 

// Send key-down then key-up

inline void sendConsumerKey(uint8_t keyBit)

{

    if (!mediaInputChar) return;

    uint8_t report[1];

 

    report[0] = keyBit;

    mediaInputChar->setValue(report, 1);

    mediaInputChar->notify();

    delay(50);

 

    report[0] = 0x00;

    mediaInputChar->setValue(report, 1);

    mediaInputChar->notify();

    delay(20);

}

 

// Send key held for a duration then release

// Used by call_control.h for answer (200ms) and decline (2000ms)

inline void sendConsumerKeyHeld(uint8_t keyBit, uint32_t holdMs)

{

    if (!mediaInputChar) return;

    uint8_t report[1];

 

    report[0] = keyBit;

    mediaInputChar->setValue(report, 1);

    mediaInputChar->notify();

    delay(holdMs);

 

    report[0] = 0x00;

    mediaInputChar->setValue(report, 1);

    mediaInputChar->notify();

    delay(20);

}

 

// Setup

void setupMediaPins()

{

    pinMode(Prev_Button,      INPUT_PULLDOWN);

    pinMode(PlayPause_Button, INPUT_PULLDOWN);

    pinMode(Next_Button,      INPUT_PULLDOWN);

    Serial.println("Media Control Pins Initialized");

}

 

// Handler -- call every loop()

// Play/Pause (pin 26) only sends BIT_PLAY_PAUSE (0xCD) -- media only.

// It does NOT send BIT_PLAY (0xB0) so it never interferes with calls.

void handleMediaControl()

{

    bool currentPrev      = digitalRead(Prev_Button);

    bool currentPlayPause = digitalRead(PlayPause_Button);

    bool currentNext      = digitalRead(Next_Button);

 

    if (prevState == LOW && currentPrev == HIGH)

    {

        Serial.println("Previous Track");

        sendConsumerKey(BIT_PREV_TRACK);

    }

 

    if (playPauseState == LOW && currentPlayPause == HIGH)

    {

        Serial.println("Play/Pause -- media only");

        sendConsumerKey(BIT_PLAY_PAUSE);

    }

 

    if (nextState == LOW && currentNext == HIGH)

    {

        Serial.println("Next Track");

        sendConsumerKey(BIT_NEXT_TRACK);

    }

 

    prevState      = currentPrev;

    playPauseState = currentPlayPause;

    nextState      = currentNext;

}

 

#endif // MEDIA_CONTROL_H