#include "Arduino.h"

#ifndef plc_standardlib_H
#define plc_standardlib_H

#define ulong unsigned long

/*
TON
*/
//!Realise a Switch-On delay.
/*!
    Code sample:

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
*/
class TON {
private:
  ulong t0;
public:
  //!Input variable
  boolean IN;
  //!Switch-on delay in milliseconds
  ulong PT;
  //! Output variable
  boolean Q;
  //! Elapsed time
  ulong ET;

  //!Constructor
  /*!
    \param pt switch-on delay in milliseconds
  */
  TON(ulong pt=0);
  //!Call cyclic for data processing
  boolean process();
  //!Call cyclic for data processing
  /*!
    \param in value for input variable
  */
  boolean process(boolean in);
};

/*
TOF
*/
//! Realise a Switch-Off delay.
/*!
Code sample:

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

*/
class TOF {
private:
  ulong t0;
public:
  //!Input variable
  boolean IN;
  //!Switch-off delay in milliseconds
  ulong PT;
  //! Output variable
  boolean Q;
  //! Elapsed time
  ulong ET;

  //!Constructor
  /*!
  \param pt switch-off delay in milliseconds
  */
  TOF(ulong pt=0);
  //!Call cyclic for data processing
  boolean process();
  //!Call cyclic for data processing
  /*!
  \param in value for input variable
  */
  boolean process(boolean in);
};

/*
R_TRIG
*/
//!Detect a rising edge.
/*!
    Code sample:

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
*/
class R_TRIG {
public:
  //!Input variable
  boolean CLK;
  //!Output variable
  boolean Q;
  //!Constructor
  R_TRIG();
  //!Call cyclic for data processing
  boolean process();
  //!Call cyclic for data processing
  /*!
    \param in value for input variable
  */
  boolean process(boolean clk);
private:
  boolean m;
};

/*
F_TRIG
*/
//!Detect a falling edge.
/*!
    Code sample:

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
*/
class F_TRIG {
public:
  //!Input variable
  boolean CLK;
  //!Output variable
  boolean Q;
  //!Constructor
  F_TRIG();
  //!Call cyclic for data processing
  boolean process();
  //!Call cyclic for data processing
  /*!
  \param in value for input variable
  */
  boolean process(boolean clk);
private:
  boolean m;
};

/*
SR
*/
//!Bistable function block with dominating set.
/*!
    Code sample:

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
*/
class SR {
public:
    boolean SET1;
    boolean RESET;
    boolean Q1;

    SR();
    boolean process();
    boolean process(boolean set1, boolean reset);
};

/*
RS
*/
//!Bistable function block with dominating reset.
/*!
Code sample:

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
*/
class RS {
public:
    boolean SET;
    boolean RESET1;
    boolean Q1;

    RS();
    boolean process();
    boolean process(boolean set, boolean reset1);
};
#endif

/*
SEMA
*/
//!Software semaphore (interruptable)
/*!
    If process() is called and SEMA::BUSY is True this means the semaphore has already
    been claimed before by some other code part. If SEMA::BUSY is False the semaphore
    has not been claimed yet or was released (SEMA::RELEASE = True).

    Code sample:

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

*/
class SEMA {
private:
    boolean x;
public:
    //!Claim the resource
    boolean CLAIM;
    //!Release the resource
    boolean RELEASE;
    //!Resource is busy if True.
    boolean BUSY;

    SEMA();
    //!Cyclic call for data processing
    boolean process();
    //!Cyclic call for data processing
    boolean process(boolean claim, boolean release);
};
