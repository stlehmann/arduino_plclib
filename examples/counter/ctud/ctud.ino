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
