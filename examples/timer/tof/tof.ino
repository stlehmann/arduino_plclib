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
