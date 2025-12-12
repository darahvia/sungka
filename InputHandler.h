// InputHandler.h

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <Arduino.h>
#include <PCF8575.h>
#include "Structures.h" 

class InputHandler {
public:
    void init();
    ButtonEvent checkButtons();

private:
    PCF8575 pcf8575;
    uint16_t lastButtonState = 0x0000; // pull-down -start with all pins LOW
    unsigned long lastDebounceTime = 0;
    const unsigned long DEBOUNCE_DELAY = 50;
    ButtonEvent mapPinToEvent(int pinIndex);
};

#endif