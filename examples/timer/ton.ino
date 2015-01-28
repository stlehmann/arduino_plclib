#include "plc_basics.h"

TON ton(1000);  // Initialise ton delay object

void setup() {
    Serial.begin(9600);
    pinMode(2, INPUT_PULLUP);
}

void loop() {
    ton.process(digitalRead(2));
    if (ton.Q) {
        Serial.println("I waited 1 second.");
    }
}
