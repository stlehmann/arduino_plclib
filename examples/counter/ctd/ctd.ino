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
