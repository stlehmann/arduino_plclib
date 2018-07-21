# Arduino plclib Library

* **version**: 1.0.2
* **license**: MIT License
* **author**: Stefan Lehmann, MrLeeh@gmx.de

# Overview

The purpose of this library is to supply basic functions and objects used in
plc programming to the Arduino world. It leans on to the functionality of the
*Standard.lib* of CoDeSys v2 defined in IEC61131-3.

I will try to include an increasing number of functions and function blocks
in this library. As this takes some time I will appreciate any help in this
matter.

Further informations about the specific functions and objects can
be found in the CoDeSys v2 manual.

The library consists of these sections:

* Bistable Latches
* Timer
* Edge detection
* Counter

# Standardlib

## Bistable Latches

### SR - Bistable function block with dominating set

**Code Sample:**

    #include "plclib.h"

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

### RS - Bistable function block with dominating reset

**Code sample:**

    #include "plclib.h"

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

### SEMA - Software semaphore (interruptable)

If SEMA::process() is called and SEMA::BUSY is True this means the semaphore has already
been claimed before by some other code part. If SEMA::BUSY is False the semaphore
has not been claimed yet or was released (SEMA::RELEASE = True).

**Code sample:**

    #include "plclib.h"
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

### TP - Impulse of a defined timespan

**Code Sample:**

    #include "plclib.h"

    #define X0 2

    TP tp(1000);  // Initialise tp impulse object
    R_TRIG rtrig;
    F_TRIG ftrig;

    void setup() {
        Serial.begin(9600);
        pinMode(X0, INPUT_PULLUP);
    }

    void loop() {
        boolean x0 = !digitalRead(X0);

        tp.process(x0);
        rtrig.process(tp.Q);
        ftrig.process(tp.Q);

        if (rtrig.Q) {
            Serial.println("Impulse comming.");
        }
        if (ftrig.Q) {
            Serial.println("Impulse going.");
        }
    }


### TON - Switch-On delay

**Code Sample:**

    #include "plclib.h"
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

### TOF - Switch-Off delay

**Code Sample:**

    #include "plclib.h"
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

### R_TRIG - Detect a rising edge

**Code Sample:**

    #include "plclib.h"
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

### F_TRIG - Detect a falling edge

**Code Sample:**

    #include "plclib.h"
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

## Counter

### CTU - Upward counter

If CTU::RESET is true the counter value CTU::CV is initialised by 0. If there
is a rising edge on CTU::CU the counter value CTU::CV is increased by 1.
CTU::Q is set to true if CTU::CV is bigger than or equal to CTU::PV.

**Code Sample:**

    #include "plclib.h"
    #define X0 2
    #define X1 3

    CTU ctu;
    R_TRIG rtrig;

    void setup() {
        pinMode(X0, INPUT_PULLUP);
        pinMode(X1, INPUT_PULLUP);
        Serial.begin(9600);
        ctu.PV = 10; //set upper limit of counter
    }

    void loop() {
        boolean x0 = !digitalRead(X0); //count up
        boolean x1 = !digitalRead(X1); //reset
        ctu.process(x0, x1);
        rtrig.process(x0 || x1);
        if (rtrig.Q) {
            Serial.print("Counter: ");
            Serial.print(ctu.CV);
            Serial.print(", ");
            Serial.print("Output: ");
            Serial.println(ctu.Q);
        }
    }

### CTD - Downward counter

If CTD::LOAD is true the counter variable CTD::CV is initialised to the
upper limit CTD::PV. If there is a rising edge on CTD::CD the counter value
CTD::CV is decreased by one as long as CTD::CV is bigger than 0.
The output variable CTD::Q is set to true if CTD::CV is equal to 0.

Code sample:

    #include "plclib.h"
    #define X0 2
    #define X1 3

    CTD ctd;
    R_TRIG rtrig;

    void setup() {
        pinMode(X0, INPUT_PULLUP);
        pinMode(X1, INPUT_PULLUP);
        Serial.begin(9600);
        ctd.PV = 10; //set upper limit of counter
    }

    void loop() {
        boolean x0 = !digitalRead(X0); //count up
        boolean x1 = !digitalRead(X1); //reset
        ctd.process(x0, x1);
        rtrig.process(x0 || x1);
        if (rtrig.Q) {
            Serial.print("Counter: ");
            Serial.print(ctd.CV);
            Serial.print(", ");
            Serial.print("Output: ");
            Serial.println(ctd.Q);
        }
    }

### CTUD - Up-and-Downward counter

If CTUD::RESET is true the counter variable CTUD::CV is initialised with 0.
If CTUD::LOAD is true the counter variable CTUD::CV is initialised with
the upper limit CTUD::PV.

If there is a rising edge on CTUD::CU the counter variable CTUD::CV
is increased by 1.
If there is a rising edge on CTUD::CD the counter variable CTUD::CV
is decreased by 1 as long as it is greater then 0.

CTUD::QU becomes true if the counter variable CTUD::CV is greater then or
equal to the upper limit CTUD::PV.
CTUD::QD becomes true if the counter variable CTUD::CV is 0.

**Code sample:**

    #include "plclib.h"
    #define X0 2
    #define X1 3

    CTUD ctud;
    R_TRIG rtrig;

    void setup() {
        pinMode(X0, INPUT_PULLUP);
        pinMode(X1, INPUT_PULLUP);
        Serial.begin(9600);
        ctud.PV = 10; //set upper limit of counter
    }

    void loop() {
        boolean x0 = !digitalRead(X0); //count up
        boolean x1 = !digitalRead(X1); //count down
        boolean rst = x0 && x1; //reset

        ctud.CU = x0;
        ctud.CD = x1;
        ctud.RESET = rst;
        ctud.process();
        rtrig.process(x0 || x1 || rst);

        if (rtrig.Q) {
            Serial.print("Counter: ");
            Serial.print(ctud.CV);
            Serial.print(", QU: ");
            Serial.print(ctud.QU);
            Serial.print(", QD: ");
            Serial.println(ctud.QD);
        }
    }
