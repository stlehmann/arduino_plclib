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
