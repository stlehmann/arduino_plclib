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
