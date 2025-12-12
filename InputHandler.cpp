// InputHandler.cpp

#include "InputHandler.h"
#include <Wire.h> 

void InputHandler::init() {
    Wire.beginTransmission(PCF8575_ADDR);
    byte error = Wire.endTransmission();
    
    if (error != 0) {
        Serial.println("ERROR: PCF8575 communication failed!");
        return;
    }
    
    pcf8575.begin(PCF8575_ADDR);
    delay(100);

    // Set ALL pins LOW
    for (int i = 0; i < 16; i++) {
        pcf8575.write(i, LOW);
    }
    
    delay(100);
    
    lastButtonState = pcf8575.read16();
    
    Serial.print("Initial button state: 0b");
    Serial.println(lastButtonState, BIN);
    Serial.println("-> InputHandler Initialized. Ready for input testing.");
}

ButtonEvent InputHandler::mapPinToEvent(int pinIndex) {
    switch (pinIndex) {
        case PIN_P1_LEFT:   return P1_LEFT;
        case PIN_P1_SELECT: return P1_SELECT;
        case PIN_P1_RIGHT:  return P1_RIGHT;
        case PIN_P2_LEFT:   return P2_LEFT;
        case PIN_P2_SELECT: return P2_SELECT;
        case PIN_P2_RIGHT:  return P2_RIGHT;
        default:            return NONE;
    }
}

ButtonEvent InputHandler::checkButtons() {
    uint16_t currentButtonState = pcf8575.read16();
    unsigned long currentTime = millis();

    uint16_t changedState = currentButtonState ^ lastButtonState;

if (changedState != 0 && (currentTime - lastDebounceTime) > DEBOUNCE_DELAY) {
    lastDebounceTime = currentTime;
        
        for (int i = 0; i < 16; i++) {

            // check if pressed (HIGH)
            if (currentButtonState & (1 << i)) {
                
                // check if was NOT pressed before (was LOW)
                if (!(lastButtonState & (1 << i))) {
                    lastButtonState = currentButtonState;
                    
                    ButtonEvent event = mapPinToEvent(i);
                    
                    Serial.print("--- BUTTON PRESSED: ");
                    Serial.print(buttonEventToString(event));
                    Serial.print(" (Pin ");
                    Serial.print(i);
                    Serial.println(")");
                    
                    return event; 
                }
            }
        }

        lastButtonState = currentButtonState;
    }

    return NONE;
}