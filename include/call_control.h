// ============================================================

//  call_control.h  --  Call Answer / Decline via Consumer HID

//  Pin 32 = Answer | Pin 33 = Decline

//  Logic: INPUT_PULLDOWN, rising-edge (LOW->HIGH) detection

//

//  These buttons are fully independent from Play/Pause (pin 26).

//  They use different Consumer keys so there is zero interference:

//

//  ANSWER / END (pin 32):

//    Sends Play (0xB0, BIT_PLAY) held 200ms.

//    0xB0 is a separate key from Play/Pause (0xCD, BIT_PLAY_PAUSE).

//    Android routes 0xB0 to the call manager when a call is active,

//    and ignores it for media when no call is present.

//    Pre-delay 100ms lets the incoming call screen gain focus.

//

//  DECLINE / SILENCE (pin 33):

//    Holds Volume Down (0xEA, BIT_VOLUME_DOWN) for 2000ms.

//    Android silences ringer on press, rejects after ~2s hold.

//    iOS: silences ringer only (Apple OS limitation).

//

//  Play/Pause (pin 26) sends 0xCD which is a completely separate

//  bit in the report -- it never triggers call answer/end.

// ============================================================

#ifndef CALL_CONTROL_H

#define CALL_CONTROL_H

 

#include <Arduino.h>

#include "media_control.h"

 

// Pin Definitions

const int Answer_Button  = 32;

const int Decline_Button = 33;

 

// Debounce States

static bool answerState  = LOW;

static bool declineState = LOW;

 

// Setup

void setupCallPins()

{

    pinMode(Answer_Button,  INPUT_PULLDOWN);

    pinMode(Decline_Button, INPUT_PULLDOWN);

    Serial.println("Call Control Pins Initialized");

}

 

// Handler -- call every loop()

void handleCallControl()

{

    bool currentAnswer  = digitalRead(Answer_Button);

    bool currentDecline = digitalRead(Decline_Button);

 

    // Answer / End call (pin 32)

    // Uses BIT_PLAY (0xB0) -- completely separate from

    // BIT_PLAY_PAUSE (0xCD) used by the Play/Pause button.

    // Held 200ms so Android call manager claims it before media session.

    if (answerState == LOW && currentAnswer == HIGH)

    {

        Serial.println("Answer / End Call (pin 32)");

        delay(100);

        sendConsumerKeyHeld(BIT_PLAY, 200);

    }

 

    // Decline / Silence incoming call (pin 33)

    // Volume Down held 2s = Android rejects the call.

    // iOS silences ringer only.

    if (declineState == LOW && currentDecline == HIGH)

    {

        Serial.println("Decline Call (pin 33)");

        sendConsumerKeyHeld(BIT_VOLUME_DOWN, 2000);

    }

 

    answerState  = currentAnswer;

    declineState = currentDecline;

}

 

#endif // CALL_CONTROL_H

