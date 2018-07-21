#include "plclib.h"
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
