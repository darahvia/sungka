#define USER_SETUP_LOADED
#include "User_Setup.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include <PCF8575.h>

#include "Constants.h"
#include "Game.h"
#include "Display.h"
#include "Input.h"
#include "AI.h"

// Hardware objects
TFT_eSPI tft = TFT_eSPI();
PCF8575 expander(0x20);

// Game objects
Game game;
Display display(&tft);
Input input(&expander);
AI ai;

// Screen management
Screen currentScreen = MENU;
int menuSelection = 0;
int rulePage = 1;

unsigned long lastAnimTime = 0;
unsigned long lastAITime = 0;


void setup() {
  // Initialize display
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  // Initialize serial
  Serial.begin(115200);
  
  // Initialize I2C
  Wire.begin();
  
  // Initialize expander
  if (!expander.begin()) {
    Serial.println("PCF8575 not found!");
    while (1);
  }
  
  // Initialize input
  input.begin();
  
  // Draw initial menu
  display.drawMenu(menuSelection);
  
  Serial.println("Sungka Game Ready!");
}

void loop() {
  // Check all buttons
  for (int i = 1; i <= 15; i++) {
    if (input.checkButton(i)) {
      handleButton(i);
    }
  }
  
  // Handle game animation
  if (currentScreen == PVP) {
    if (millis() - lastAnimTime > ANIM_DELAY) {
      bool needsRedraw = false;
      
      if (game.p1State == ANIMATING) {
        game.animateP1();
        needsRedraw = true;
      }
      
      if (game.p2State == ANIMATING) {
        game.animateP2();
        needsRedraw = true;
      }
      
      if (needsRedraw) {
        display.drawGameBoard(&game);
      }

      if (game.gameOver) {
        String msg = "DRAW!";
        int w = game.getWinner();
        if (w == 1) msg = "P1 WINS!";
        else if (w == 2) msg = "P2 WINS!";
        

        tft.fillRect(60, 100, 200, 50, TFT_BLACK);
        tft.drawRect(60, 100, 200, 50, TFT_WHITE);
        
        tft.setTextSize(3);
        tft.setTextColor(TFT_MAGENTA);
        int textX = 160 - (wMsg.length() * 9); 
        tft.setCursor(textX, 115);
        tft.print(msg);
        
        delay(5000);
        
        game.gameOver = false; 
        currentScreen = MENU;
        menuSelection = 0;
        display.drawMenu(menuSelection);
      }
      lastAnimTime = millis();
    }
  }
  
  // Handle PvAI game animation and AI moves
if (currentScreen == PVA) {
    // Animation
    if (millis() - lastAnimTime > ANIM_DELAY) {
      bool needsRedraw = false;
      if (game.p1State == ANIMATING) {
        game.animateP1();
        needsRedraw = true;
      }
      
      if (game.p2State == ANIMATING) {
        game.animateP2();
        needsRedraw = true;
      }
      
      if (needsRedraw) {
        display.drawPvAIGameBoard(&game);
      }

      if (game.gameOver) {
        String wMsg = "DRAW!";
        int w = game.getWinner();
        if (w == 1) msg = "YOU WIN!";
        else if (w == 2) msg = "AI WINS!";
        
        game.lastRuleMsg = msg; 
        display.drawPvAIGameBoard(&game); 
        
        tft.fillRect(60, 100, 200, 50, TFT_BLACK);
        tft.drawRect(60, 100, 200, 50, TFT_WHITE);
        tft.setTextSize(3);
        tft.setTextColor(TFT_MAGENTA);
        int textX = 160 - (msg.length() * 9);
        tft.setCursor(textX, 115);
        tft.print(msg);

        delay(5000);
        
        game.gameOver = false;
        currentScreen = MENU;
        menuSelection = 0;
        display.drawMenu(menuSelection);
      }

      lastAnimTime = millis();
    }
    

// ai
    if (game.p2State == SELECTING) {

      if (millis() - lastAITime > AI_MOVE_DELAY) {
        
        // Get random valid move
        int aiMove = ai.getMove(&game);
        
        if (aiMove != -1) {
          game.p2Selected = aiMove;
          
          if (game.firstMove) {
            game.p2State = READY;
            
            if (game.p1State == READY) {
               game.startFirstMove();
            }
          } else {
            game.startP2Move();
          }
          
          display.drawPvAIGameBoard(&game);
        }
        lastAITime = millis();
      }
    }
  }
  
  delay(BUTTON_DELAY);
}
void handleButton(int btn) {
  if (currentScreen == MENU) {
    input.handleMenuButton(btn, &menuSelection, &currentScreen);
    
    if (currentScreen == MENU) {
      display.drawMenu(menuSelection);
    } else if (currentScreen == PVP) {
      game.init();
      display.drawGameBoard(&game);
    } else if (currentScreen == PVA) {
      game.init();
      display.drawPvAIGameBoard(&game);
      lastAITime = millis();
    } else if (currentScreen == RULES) {
      display.drawRulesScreen(rulePage);
    }
  }
  else if (currentScreen == PVP) {
    input.handleGameButton(btn, &game);
    display.drawGameBoard(&game);
  }
  else if (currentScreen == PVA) {
    if (btn == BTN_P1_LEFT || btn == BTN_P1_RIGHT || btn == BTN_P1_SELECT) {
      input.handleGameButton(btn, &game);
      display.drawPvAIGameBoard(&game);
    }
    else if (btn == BTN_P2_SELECT) {
      currentScreen = MENU;
      menuSelection = 0;
      display.drawMenu(menuSelection);
    }
  }
else if (currentScreen == RULES) {
    if (btn == BTN_P1_RIGHT || btn == BTN_P2_RIGHT || btn == BTN_P1_SELECT || btn == BTN_P2_SELECT) {
      if (rulePage < 3) {
        rulePage++;
        display.drawRulesScreen(rulePage);
      } else {
        currentScreen = MENU;
        menuSelection = 0;
        display.drawMenu(menuSelection);
      }
    }
    else if (btn == BTN_P1_LEFT || btn == BTN_P2_LEFT) {
      if (rulePage > 1) {
        rulePage--;
        display.drawRulesScreen(rulePage);
      } else {
        currentScreen = MENU;
        menuSelection = 0;
        display.drawMenu(menuSelection);
      }
    }
  }
  else {
    input.handleBackButton(btn, &currentScreen, &menuSelection);
    if (currentScreen == MENU) {
      display.drawMenu(menuSelection);
    }
  }
}

// #include <Wire.h>
// #include <PCF8575.h>

// // Initialize PCF8575 with default address 0x20
// PCF8575 expander(0x20);

// // --- PIN DEFINITIONS ---

// // MOVED: P1 Right is now on ESP32 directly
// // We use GPIO 4 because GPIO 3 is reserved for Serial RX (USB)
// const int BTN_P1_RIGHT_ESP = 4; 

// // PCF8575 PIN MAPPING (0-15)
// // Note: Pin 3 is now unused on the expander
// #define BTN_P1_LEFT    12 
// #define BTN_P1_SELECT  13 
// // #define BTN_P1_RIGHT   3  <-- REMOVED from PCF
// #define BTN_P2_LEFT    15 
// #define BTN_P2_SELECT  7 
// #define BTN_P2_RIGHT   14 

// // Other mapped pins
// #define one     1
// #define two     2
// // pin 3 skipped
// #define four    4
// #define five    5
// #define six     6
// #define seven   7 // Duplicate of P2_SELECT in your list, effectively same pin
// #define eight   8
// #define nine    9
// #define eleven  11
// #define sixteen 16 // PCF8575 only has 0-15. This is likely an error in your original list. 
//                    // If you meant Pin 0, change to 0. 

// // State Tracking
// bool lastStatePCF[16] = {0}; 
// bool lastStateESP = LOW;

// void setup() {
//   Serial.begin(115200);
//   Wire.begin();
  
//   // --- 1. SETUP ESP32 PIN ---
//   // Using INPUT_PULLDOWN adds an internal backup resistor 
//   // just in case your external resistor is loose.
//   pinMode(BTN_P1_RIGHT_ESP, INPUT_PULLDOWN);

//   // --- 2. SETUP PCF8575 ---
//   if (!expander.begin()) {
//     Serial.println("PCF8575 not found!");
//     while (1);
//   }
  
//   // CRITICAL FIX: To use PCF8575 pins as INPUTS, you must write HIGH.
//   // This turns on the weak internal pull-up. 
//   // Your external pull-down resistor (1k-3k) will overpower this 
//   // and pull the pin LOW until the button is pressed.
//   for (int i = 0; i < 16; i++) {
//     expander.write(i, HIGH);
//   }
  
//   Serial.println("Sungka Button Test Ready!");
//   Serial.println("System: External Pull-Downs (Active HIGH)");
// }

// void loop() {
//   // --- A. READ ESP32 DIRECT BUTTON ---
//   bool currentEspState = digitalRead(BTN_P1_RIGHT_ESP);
  
//   if (currentEspState == HIGH && lastStateESP == LOW) {
//     Serial.println("Button pressed: P1 RIGHT (ESP32 Pin 4)");
//   }
//   lastStateESP = currentEspState;

//   // --- B. READ PCF8575 BUTTONS ---
//   for (int i = 0; i < 16; i++) {
//     // Skip Pin 3 (physically moved) and Pin 16 (doesn't exist on PCF8575)
//     if (i == 3) continue; 
    
//     // Read the pin
//     // If you used strong pull-down resistors, LOW = Unpressed, HIGH = Pressed
//     bool currentState = expander.read(i);
    
//     if (currentState == HIGH && lastStatePCF[i] == LOW) {
//       printExpanderButton(i);
//     }
    
//     lastStatePCF[i] = currentState;
//   }
  
//   delay(50); // Short debounce
// }

// void printExpanderButton(int btn) {
//   Serial.print("Button pressed: ");

//   switch (btn) {
//     case BTN_P1_LEFT:    Serial.println("P1 LEFT"); break;
//     case BTN_P1_SELECT:  Serial.println("P1 SELECT"); break;
//     // case 3: handled by ESP32 logic above
//     case BTN_P2_LEFT:    Serial.println("P2 LEFT"); break;
//     case BTN_P2_SELECT:  Serial.println("P2 SELECT"); break;
//     case BTN_P2_RIGHT:   Serial.println("P2 RIGHT"); break;

//     case one:            Serial.println("ONE"); break;
//     case two:            Serial.println("TWO"); break;
//     case four:           Serial.println("FOUR"); break;
//     case five:           Serial.println("FIVE"); break;
//     case six:            Serial.println("SIX"); break;
//     case eight:          Serial.println("EIGHT"); break;
//     case nine:           Serial.println("NINE"); break;
//     case eleven:         Serial.println("ELEVEN"); break;
    
//     default:
//       Serial.print("PIN ");
//       Serial.println(btn);
//   }
// }