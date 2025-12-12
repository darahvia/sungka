// #include <Wire.h>
// #include <PCF8575.h>

// // Initialize PCF8575 with default address 0x20
// PCF8575 expander(0x20);

// // Button pins on PCF8575
// #define BTN_P1_LEFT   12 //14
// #define BTN_P1_SELECT 13 //15
// #define BTN_P1_RIGHT  3
// #define BTN_P2_LEFT   14  // 16
// #define BTN_P2_SELECT 10 // 12
// #define BTN_P2_RIGHT  15 // 17

// // Button states
// bool lastState[16] = {0}; // Index 0 unused, 1-6 for buttons

// void setup() {
//   Serial.begin(115200);
//   Wire.begin();
  
//   // Initialize PCF8575
//   if (!expander.begin()) {
//     Serial.println("PCF8575 not found!");
//     while (1);
//   }
  
//   // Set all pins as INPUT with pull-down (LOW)
//   for (int i = 1; i <= 15; i++) {
//     expander.write(i, LOW);
//   }
  
//   Serial.println("Sungka Button Test Ready!");
//   Serial.println("Press any button...");
// }

// void loop() {
//   // Read all button states
//   for (int i = 1; i <= 15; i++) {
//     bool currentState = expander.read(i);
    
//     // Check for button press (active HIGH with pull-down)
//     if (currentState == HIGH && lastState[i] == LOW) {
//       printButton(i);
//     }
    
//     lastState[i] = currentState;
//   }
  
//   delay(100); // Debounce delay
// }

// void printButton(int btn) {
//   Serial.print("Button pressed: ");
  
//   switch(btn) {
//     case BTN_P1_LEFT:
//       Serial.println("P1 LEFT");
//       break;
//     case BTN_P1_SELECT:
//       Serial.println("P1 SELECT");
//       break;
//     case BTN_P1_RIGHT:
//       Serial.println("P1 RIGHT");
//       break;
//     case BTN_P2_LEFT:
//       Serial.println("P2 LEFT");
//       break;
//     case BTN_P2_SELECT:
//       Serial.println("P2 SELECT");
//       break;
//     case BTN_P2_RIGHT:
//       Serial.println("P2 RIGHT");
//       break;
//     default:
//       Serial.println("UNKNOWN");
//   }
// }


// THE FIRST CODE ABOVE IS USED FOR DETECTING BUTTONS LANG

#include <Arduino.h>
#include <Wire.h> // Required for I2C communication (PCF8575)


// devkitc4 + expander
const int I2C_SDA = 21; 
const int I2C_SCL = 22; 

#include "Structures.h" 
#include "InputHandler.h"
#include "GameDisplay.h" 
#include "GameController.h"

// global instances
GameDisplay display; 
InputHandler inputHandler;
GameController controller;

void setup() {
    Serial.begin(115200);    
    Serial.println("--- Sungka Game Initialization (ESP32) ---");

    // init i2c comms
    Wire.begin(I2C_SDA, I2C_SCL); 

    inputHandler.init();
    display.init(); 


    controller.init(&display, &inputHandler); 
    controller.startGame(); 
}

void loop() {
    ButtonEvent event = inputHandler.checkButtons();
    
    if (event != NONE) {
        controller.handleButtonPress(event);
    }
    
    controller.update();
}