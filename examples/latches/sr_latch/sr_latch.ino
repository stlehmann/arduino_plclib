#include "plclib.h"
#define X0 2
#define X1 3

SR sr_latch;
R_TRIG rtrig;
F_TRIG ftrig;

void setup() {
    pinMode(X0, INPUT_PULLUP);
    pinMode(X1, INPUT_PULLUP);
    Serial.begin(9600);
}

void loop() {
    boolean x0 = !digitalRead(X0); //set switch
    boolean x1 = !digitalRead(X1); //reset switch

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
