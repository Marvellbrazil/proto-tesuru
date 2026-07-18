#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Arduino.h>

#define LEDPIN 2

enum class BLINKPATTERN {
    SLOW,
    FAST,
    DOUBLE_BLINK
};

void initLED();
void activateLED();
void deactivateLED();
void executeLEDBlinkPattern(BLINKPATTERN pattern);

#endif