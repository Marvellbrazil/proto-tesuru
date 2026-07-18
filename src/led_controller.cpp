#include "led_controller.h"

void initLED() {
    pinMode(LEDPIN, OUTPUT);
    digitalWrite(LEDPIN, LOW);
}

void activateLED() {
    digitalWrite(LEDPIN, HIGH);
}

void deactivateLED() {
    digitalWrite(LEDPIN, LOW);
}

void executeLEDBlinkPattern(BLINKPATTERN pattern) {
    switch (pattern)
    {
    case BLINKPATTERN::SLOW:
        digitalWrite(LEDPIN, HIGH);
        delay(1000);
        digitalWrite(LEDPIN, LOW);
        delay(1000);
        break;
        
    case BLINKPATTERN::FAST:
        digitalWrite(LEDPIN, HIGH);
        delay(200);
        digitalWrite(LEDPIN, LOW);
        delay(200);
        break;
        
    case BLINKPATTERN::DOUBLE_BLINK:
        digitalWrite(LEDPIN, HIGH);
        delay(150);
        digitalWrite(LEDPIN, LOW);
        delay(150);
        digitalWrite(LEDPIN, HIGH);
        delay(150);
        digitalWrite(LEDPIN, LOW);
        delay(750);
        break;
    }
}