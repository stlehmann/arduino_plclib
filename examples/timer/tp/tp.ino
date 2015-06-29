#include "plclib.h"

#define X0 2

TP tp(1000);  // Initialise ton delay object
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
