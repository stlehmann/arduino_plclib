#include "plclib.h"
#define X0 2

R_TRIG rtrig; //detect rising edges
F_TRIG ftrig; //detect falling edges

void setup() {
    pinMode(X0, INPUT_PULLUP);
    Serial.begin(9600);
}

void loop() {
    boolean x0 = !digitalRead(X0);
    rtrig.process(x0);
    ftrig.process(x0);
    if (rtrig.Q) {
        Serial.println("This was a rising edge.");
    }
    if (ftrig.Q) {
        Serial.println("This was a falling edge.");
    }
}
