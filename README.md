# plc_standardlib Library

# Overview

The purpose of this library is to supply basic functions and objects used in
plc programming to the Arduino world. It refers to the functionality of the
*Standard.lib* of 3S CoDeSys v2. This library contains functions and
function blocks claimed by IEC61131-3.

I will try to include an increasing number of functions and function blocks
in this library. As this takes some time I will appreciate any help in this
matter.

Further informations about the specific functions and objects can
be found in the CoDeSys v2 manual.

The library consists of these sections:

* Bistable Latches
* Timer
* Edge detection

# Standardlib

## Bistable Latches

### SR

Bistable function block with dominating set.

**Code Sample:**

    #include "plc_standardlib.h"

    SR sr_latch;
    R_TRIG rtrig;
    F_TRIG ftrig;

    void setup() {
        pinMode(0, INPUT_PULLUP);
        pinMode(1, INPUT_PULLUP);
        Serial.begin(9600);
    }

    void loop() {
        boolean x0 = !digitalRead(0); //set switch
        boolean x1 = !digitalRead(1); //reset switch

        sr_latch.process(x0, x1);
        rtrig.process(sr_latch.Q1);
        ftrig.process(sr_latch.Q1);

        if (rtrig.Q) {
            Serial.println("On");
        }
        if (ftrig.Q) {
            Serial.println("Off");
        }
    }

### RS

Bistable function block with dominating reset.

**Code sample:**

    #include "plc_standardlib.h"

    RS rs_latch;
    R_TRIG rtrig;
    F_TRIG ftrig;

    void setup() {
        pinMode(0, INPUT_PULLUP);
        pinMode(1, INPUT_PULLUP);
        Serial.begin(9600);
    }

    void loop() {
        boolean x0 = !digitalRead(0); //set switch
        boolean x1 = !digitalRead(1); //reset switch

        rs_latch.process(x0, x1);
        rtrig.process(rs_latch.Q1);
        ftrig.process(rs_latch.Q1);

        if (rtrig.Q) {
            Serial.println("On");
        }
        if (ftrig.Q) {
            Serial.println("Off");
        }
    }

### SEMA

Software semaphore (interruptable)
If SEMA::process() is called and SEMA::BUSY is True this means the semaphore has already
been claimed before by some other code part. If SEMA::BUSY is False the semaphore
has not been claimed yet or was released (SEMA::RELEASE = True).

**Code sample:**

    #include "plc_standardlib.h"
    #define X0 2
    #define X1 3

    SEMA semaphore;
    R_TRIG trigger;

    void setup() {
        pinMode(X0, INPUT_PULLUP);
        pinMode(X1, INPUT_PULLUP);
        Serial.begin(9600);
    }

    void loop() {
        boolean x0 = !digitalRead(X0); //claim
        boolean x1 = !digitalRead(X1); //release
        boolean was_busy = semaphore.BUSY;
        semaphore.CLAIM = x0;
        semaphore.RELEASE = x1;
        semaphore.process();
        trigger.process(x0 || x1);
        if (trigger.Q) {
            if (x0) {
                if (semaphore.BUSY) {
                    Serial.println("Resource is busy.");
                }
                else {
                    Serial.println("Resource claimed successfully.");
                }
            }
            if (x1) {
                if (was_busy) {
                    Serial.println("Resource released.");
                }
                else {
                    Serial.println("Nothing to release.");
                }
            }
        }
    }


## Timer

### TON

Realise a Switch-On delay.

**Code Sample:**

    #include "plc_standardlib.h"
    #define X0 2

    TON ton(1000);  // Initialise ton delay object
    R_TRIG rtrig;

    void setup() {
        Serial.begin(9600);
        pinMode(X0, INPUT_PULLUP);
    }

    void loop() {
        boolean x0 = !digitalRead(X0);
        ton.process(x0);
        rtrig.process(ton.Q);
        if (rtrig.Q) {
            Serial.println("I waited 1 second.");
        }
    }

### TOF

Realise a Switch-Off delay.

**Code Sample:**

    #include "plc_standardlib.h"
    #define X0 2

    TOF tof(1000);  // Initialise ton delay object
    F_TRIG ftrig;

    void setup() {
        Serial.begin(9600);
        pinMode(X0, INPUT_PULLUP);
    }

    void loop() {
        boolean x0 = !digitalRead(X0);
        tof.process(x0);
        ftrig.process(tof.Q);
        if (ftrig.Q) {
            Serial.println("I waited 1 second.");
        }
    }

## Edge detection

### R_TRIG

Detect a rising edge.

**Code Sample:**

    #include "plc_standardlib.h"
    #define X0 2

    R_TRIG trig;
    void setup() {
        pinMode(X0, INPUT_PULLUP);
        Serial.begin(9600);
    }

    void loop() {
        boolean x0 = !digitalRead(X0);
        trig.process(x0);
        if (trig.Q) {
            Serial.println("This was a rising edge.");
        }
    }

### F_TRIG
Detect a falling edge.

** Code Sample:**

    #include "plc_standardlib.h"
    #define X0 2

    F_TRIG trig;
    void setup() {
        pinMode(X0, INPUT_PULLUP);
        Serial.begin(9600);
    }

    void loop() {
        boolean x0 = !digitalRead(X0);
        trig.process(x0);
        if (trig.Q) {
            Serial.println("This was a falling edge.");
        }
    }
