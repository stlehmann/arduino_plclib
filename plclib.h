#ifndef plc_standardlib_H
#define plc_standardlib_H

#define ulong unsigned long
#define uint unsigned int
/*
TP
*/
//!Realise an impulse of a defined length.
/*!
    If TP::IN gets True TP::ET will be increased until it equals TP::PT,
    then it stays constant. TP::Q becomes True if TP::IN changed to True
    and TP::ET is smaller then TP::PT. Otherwise TP::Q is False.
    So TP::Q provides an impulse during the impulse time TP::PT.

    Code sample:

        #include "plc_standardlib.h"

        #define X0 2

        TP tp(1000);  // Initialise tp impulse object
        R_TRIG rtrig;
        F_TRIG ftrig;

        void setup() {
            Serial.begin(9600);
            pinMode(X0, INPUT_PULLUP);
        }

        void loop() {
            bool x0 = !digitalRead(X0);

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
*/
class TP {
private:
    ulong t0;
public:
    //!Input variable
    bool IN;
    //!Impulse length in milliseconds
    ulong PT;
    //!Output variable
    bool Q;
    //!Elapsed time in milliseconds
    ulong ET;

    //!Constructor
    TP();
    //!Constructor with inpulse length
    TP(ulong pt);
    //!Cyclic for data processing
    bool process();
    //!Cyclic for data processing with input parameter
    bool process(bool in);
};


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
            bool x0 = !digitalRead(X0);
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
  bool IN;
  //!Switch-on delay in milliseconds
  ulong PT;
  //! Output variable
  bool Q;
  //! Elapsed time
  ulong ET;

  //!Constructor
  /*!
    \param pt switch-on delay in milliseconds
  */
  TON(ulong pt=0);
  //!Call cyclic for data processing
  bool process();
  //!Call cyclic for data processing
  /*!
    \param in value for input variable
  */
  bool process(bool in);
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
        bool x0 = !digitalRead(X0);
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
  bool IN;
  //!Switch-off delay in milliseconds
  ulong PT;
  //! Output variable
  bool Q;
  //! Elapsed time
  ulong ET;

  //!Constructor
  /*!
  \param pt switch-off delay in milliseconds
  */
  TOF(ulong pt=0);
  //!Call cyclic for data processing
  bool process();
  //!Call cyclic for data processing
  /*!
  \param in value for input variable
  */
  bool process(bool in);
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
            bool x0 = !digitalRead(X0);
            trig.process(x0);
            if (trig.Q) {
                Serial.println("This was a rising edge.");
            }
        }
*/
class R_TRIG {
public:
  //!Input variable
  bool CLK;
  //!Output variable
  bool Q;
  //!Constructor
  R_TRIG();
  //!Call cyclic for data processing
  bool process();
  //!Call cyclic for data processing
  /*!
    \param in value for input variable
  */
  bool process(bool clk);
private:
  bool m;
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
            bool x0 = !digitalRead(X0);
            trig.process(x0);
            if (trig.Q) {
                Serial.println("This was a falling edge.");
            }
        }
*/
class F_TRIG {
public:
  //!Input variable
  bool CLK;
  //!Output variable
  bool Q;
  //!Constructor
  F_TRIG();
  //!Call cyclic for data processing
  bool process();
  //!Call cyclic for data processing
  /*!
  \param in value for input variable
  */
  bool process(bool clk);
private:
  bool m;
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
            bool x0 = !digitalRead(0); //set switch
            bool x1 = !digitalRead(1); //reset switch

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
    bool SET1;
    bool RESET;
    bool Q1;

    SR();
    bool process();
    bool process(bool set1, bool reset);
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
        bool x0 = !digitalRead(0); //set switch
        bool x1 = !digitalRead(1); //reset switch

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
    bool SET;
    bool RESET1;
    bool Q1;

    RS();
    bool process();
    bool process(bool set, bool reset1);
};

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
            bool x0 = !digitalRead(X0); //claim
            bool x1 = !digitalRead(X1); //release
            bool was_busy = semaphore.BUSY;
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
    bool x;
public:
    //!Claim the resource
    bool CLAIM;
    //!Release the resource
    bool RELEASE;
    //!Resource is busy if True.
    bool BUSY;

    SEMA();
    //!Cyclic call for data processing
    bool process();
    //!Cyclic call for data processing
    bool process(bool claim, bool release);
};

// === COUNTER ===
/*
CTU
*/
//!Upward counter
/*!
    If CTU::RESET is true the counter value CTU::CV is initialised by 0. If there
    is a rising edge on CTU::CU the counter value CTU::CV is increased by 1.
    CTU::Q is set to true if CTU::CV is bigger than or equal to CTU::PV.

    Code sample:

        #include "plc_standardlib.h"
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
            bool x0 = !digitalRead(X0); //count up
            bool x1 = !digitalRead(X1); //reset
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
*/
class CTU {
private:
    bool cu_old;
public:
    //!Input variable for counting up
    bool CU;
    //!Input variable for reset
    bool RESET;
    //!Output variable limit reached
    bool Q;
    //!Counter limit
    uint PV;
    //!Current counter value
    uint CV;

    //!Constructor
    CTU(uint pv=0);
    //!Cyclic call for data processing
    bool process();
    //!Cyclic call for data processing with input variables
    bool process(bool cu, bool reset);
};

/*
CTD
*/
//!Downward counter
/*!
    If CTD::LOAD is true the counter variable CTD::CV is initialised to the
    upper limit CTD::PV. If there is a rising edge on CTD::CD the counter value
    CTD::CV is decreased by one as long as CTD::CV is bigger than 0.
    The output variable CTD::Q is set to true if CTD::CV is equal to 0.

    Code sample:
        #include "plc_standardlib.h"
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
            bool x0 = !digitalRead(X0); //count up
            bool x1 = !digitalRead(X1); //reset
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

*/
class CTD {
private:
    bool cd_old;
public:
    //!Input variable for counting down
    bool CD;
    //!Input variable for loading the upper limit CTD::PV
    bool LOAD;
    //!Output variable
    bool Q;
    //!Upper counter  limit
    uint PV;
    //!Current counter value
    uint CV;

    //!Constructor
    CTD(uint pv=0);
    //!Cyclic call for data processing
    bool process();
    //!Cyclic call for data processing with input variables
    bool process(bool cd, bool load);
};

/*
CTUD
*/
//!Up- and Downward counter
/*!
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

    Code sample:

        #include "plc_standardlib.h"
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
            bool x0 = !digitalRead(X0); //count up
            bool x1 = !digitalRead(X1); //count down
            bool rst = x0 && x1; //reset

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
*/
class CTUD {
private:
    bool cu_old;
    bool cd_old;
public:
    //!Input variable for counting up
    bool CU;
    //!Input variable for counting down
    bool CD;
    //!Input variable for resetting counter value to 0
    bool RESET;
    //!Input variable for loading the upper limit CTD::PV
    bool LOAD;
    //!Output variable upper limit reached
    bool QU;
    //!Output varialbe lower limit reached
    bool QD;
    //!Upper counter limit
    uint PV;
    //!Current counter value
    uint CV;

    //!Constructor
    CTUD(uint pv=0);
    //!Cyclic call for data processing
    bool process();
    //!Cyclic call for data processing with input variables
    bool process(bool cu, bool cd, bool reset, bool load);
};


unsigned long plclib_millis();
#endif
