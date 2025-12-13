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

// Hardware objects
TFT_eSPI tft = TFT_eSPI();
PCF8575 expander(0x20);

// Game objects
Game game;
Display display(&tft);
Input input(&expander);

// Screen management
Screen currentScreen = MENU;
int menuSelection = 0;

unsigned long lastAnimTime = 0;

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
      
      lastAnimTime = millis();
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
      display.drawPvAIScreen();
    } else if (currentScreen == LEADERBOARD) {
      display.drawLeaderboardScreen();
    }
  }
  else if (currentScreen == PVP) {
    input.handleGameButton(btn, &game);
    display.drawGameBoard(&game);
  }
  else {
    input.handleBackButton(btn, &currentScreen, &menuSelection);
    if (currentScreen == MENU) {
      display.drawMenu(menuSelection);
    }
  }
}
// #define USER_SETUP_LOADED
// #include "User_Setup.h"
// #include <SPI.h>
// #include <TFT_eSPI.h>
// #include <Wire.h>
// #include <PCF8575.h>

// TFT_eSPI tft = TFT_eSPI();
// bool lastState[16] = {0};
// PCF8575 expander(0x20);

// // Button pins on PCF8575
// #define BTN_P1_LEFT   12
// #define BTN_P1_SELECT 13
// #define BTN_P1_RIGHT  3
// #define BTN_P2_LEFT   14
// #define BTN_P2_SELECT 7
// #define BTN_P2_RIGHT  15

// // Menu states
// enum Screen {
//   MENU,
//   PVP,
//   PVA,
//   LEADERBOARD
// };

// enum PlayerState {
//   SELECTING,
//   READY,
//   ANIMATING,
//   WAITING
// };

// Screen currentScreen = MENU;
// int menuSelection = 0;

// // Game variables
// int pits[16]; // 0=P1 store, 1-7=P1 pits, 8=P2 store, 9-15=P2 pits
// bool firstMove = true;

// // P1 state
// PlayerState p1State = SELECTING;
// int p1Selected = -1;
// int p1CurrentPos = -1;
// int p1SeedsInHand = 0;

// // P2 state
// PlayerState p2State = SELECTING;
// int p2Selected = -1;
// int p2CurrentPos = -1;
// int p2SeedsInHand = 0;

// unsigned long lastAnimTime = 0;

// void setup() {
//   tft.init();
//   tft.setRotation(0);
//   tft.fillScreen(TFT_BLACK);

//   Serial.begin(115200);
//   Wire.begin();
  
//   if (!expander.begin()) {
//     Serial.println("PCF8575 not found!");
//     while (1);
//   }
  
//   for (int i = 1; i <= 15; i++) {
//     expander.write(i, LOW);
//   }
  
//   drawMenu();
//   Serial.println("Sungka Game Ready!");
// }

// void loop() {
//   for (int i = 1; i <= 15; i++) {
//     bool currentState = expander.read(i);
    
//     if (currentState == HIGH && lastState[i] == LOW) {
//       handleButton(i);
//     }
    
//     lastState[i] = currentState;
//   }
  
//   // Handle animation
//   if (currentScreen == PVP) {
//     if (millis() - lastAnimTime > 300) {
//       bool needsRedraw = false;
      
//       if (p1State == ANIMATING) {
//         animateP1();
//         needsRedraw = true;
//       }
      
//       if (p2State == ANIMATING) {
//         animateP2();
//         needsRedraw = true;
//       }
      
//       if (needsRedraw) {
//         drawGameBoard();
//       }
      
//       lastAnimTime = millis();
//     }
//   }
  
//   delay(50);
// }

// void handleButton(int btn) {
//   if (currentScreen == MENU) {
//     if (btn == BTN_P1_LEFT || btn == BTN_P2_LEFT) {
//       menuSelection--;
//       if (menuSelection < 0) menuSelection = 2;
//       drawMenu();
//     }
//     else if (btn == BTN_P1_RIGHT || btn == BTN_P2_RIGHT) {
//       menuSelection++;
//       if (menuSelection > 2) menuSelection = 0;
//       drawMenu();
//     }
//     else if (btn == BTN_P1_SELECT || btn == BTN_P2_SELECT) {
//       if (menuSelection == 0) {
//         currentScreen = PVP;
//         initGame();
//         drawGameBoard();
//       }
//       else if (menuSelection == 1) {
//         currentScreen = PVA;
//         drawPvAIScreen();
//       }
//       else if (menuSelection == 2) {
//         currentScreen = LEADERBOARD;
//         drawLeaderboardScreen();
//       }
//     }
//   }
//   else if (currentScreen == PVP) {
//     // Handle P1 buttons
//     if (btn == BTN_P1_LEFT || btn == BTN_P1_RIGHT || btn == BTN_P1_SELECT) {
//       handleP1Button(btn);
//     }
//     // Handle P2 buttons
//     else if (btn == BTN_P2_LEFT || btn == BTN_P2_RIGHT || btn == BTN_P2_SELECT) {
//       handleP2Button(btn);
//     }
//   }
//   else {
//     if (btn == BTN_P1_SELECT || btn == BTN_P2_SELECT) {
//       currentScreen = MENU;
//       menuSelection = 0;
//       drawMenu();
//     }
//   }
// }

// void handleP1Button(int btn) {
//   if (p1State == SELECTING) {
//     if (btn == BTN_P1_LEFT) {
//       p1Selected--;
//       if (p1Selected < 1) p1Selected = 7;
//       drawGameBoard();
//     }
//     else if (btn == BTN_P1_RIGHT) {
//       p1Selected++;
//       if (p1Selected > 7) p1Selected = 1;
//       drawGameBoard();
//     }
//     else if (btn == BTN_P1_SELECT && p1Selected != -1) {
//       if (firstMove) {
//         p1State = READY;
//         drawGameBoard();
//         // Check if both ready
//         if (p2State == READY) {
//           startFirstMove();
//         }
//       } else {
//         startP1Move();
//       }
//     }
//   }
// }

// void handleP2Button(int btn) {
//   if (p2State == SELECTING) {
//     if (btn == BTN_P2_LEFT) {
//       p2Selected--;
//       if (p2Selected < 9) p2Selected = 15;
//       drawGameBoard();
//     }
//     else if (btn == BTN_P2_RIGHT) {
//       p2Selected++;
//       if (p2Selected > 15) p2Selected = 9;
//       drawGameBoard();
//     }
//     else if (btn == BTN_P2_SELECT && p2Selected != -1) {
//       if (firstMove) {
//         p2State = READY;
//         drawGameBoard();
//         // Check if both ready
//         if (p1State == READY) {
//           startFirstMove();
//         }
//       } else {
//         startP2Move();
//       }
//     }
//   }
// }

// void initGame() {
//   // Initialize pits: stores = 0, playing pits = 7
//   pits[0] = 0;  // P1 store
//   pits[8] = 0;  // P2 store
//   for (int i = 1; i <= 7; i++) {
//     pits[i] = 7;  // P1 pits
//   }
//   for (int i = 9; i <= 15; i++) {
//     pits[i] = 7;  // P2 pits
//   }
  
//   firstMove = true;
//   p1State = SELECTING;
//   p2State = SELECTING;
//   p1Selected = 1;
//   p2Selected = 9;
// }

// void startFirstMove() {
//   firstMove = false;
  
//   // Pick up seeds for both players
//   p1SeedsInHand = pits[p1Selected];
//   pits[p1Selected] = 0;
//   p1CurrentPos = p1Selected;
//   p1State = ANIMATING;
  
//   p2SeedsInHand = pits[p2Selected];
//   pits[p2Selected] = 0;
//   p2CurrentPos = p2Selected;
//   p2State = ANIMATING;
  
//   drawGameBoard();
// }

// void startP1Move() {
//   p1SeedsInHand = pits[p1Selected];
//   pits[p1Selected] = 0;
//   p1CurrentPos = p1Selected;
//   p1State = ANIMATING;
//   drawGameBoard();
// }

// void startP2Move() {
//   p2SeedsInHand = pits[p2Selected];
//   pits[p2Selected] = 0;
//   p2CurrentPos = p2Selected;
//   p2State = ANIMATING;
//   drawGameBoard();
// }

// void animateP1() {
//   if (p1SeedsInHand > 0) {
//     p1CurrentPos = getNextPos(p1CurrentPos, true);
//     pits[p1CurrentPos]++;
//     p1SeedsInHand--;
    
//     if (p1SeedsInHand == 0) {
//       p1State = SELECTING;
//       p1Selected = 1;
//     }
//   }
// }

// void animateP2() {
//   if (p2SeedsInHand > 0) {
//     p2CurrentPos = getNextPos(p2CurrentPos, false);
//     pits[p2CurrentPos]++;
//     p2SeedsInHand--;
    
//     if (p2SeedsInHand == 0) {
//       p2State = SELECTING;
//       p2Selected = 9;
//     }
//   }
// }

// int getNextPos(int pos, bool isP1) {
//   // Counter-clockwise movement
//   if (pos >= 1 && pos <= 7) {
//     if (pos == 7) return 8;
//     return pos + 1;
//   }
//   else if (pos == 8) {
//     return 15;
//   }
//   else if (pos >= 9 && pos <= 15) {
//     if (pos == 9) return 0;
//     return pos - 1;
//   }
//   else if (pos == 0) {
//     if (isP1) return 1;
//     return 7;
//   }
  
//   return pos;
// }

// void drawButton(int x, int y, int w, int h, String text, bool selected) {
//   tft.fillRoundRect(x + 3, y + 3, w, h, 8, TFT_DARKGREY);
  
//   if (selected) {
//     tft.fillRoundRect(x, y, w, h, 8, TFT_ORANGE);
//     tft.drawRoundRect(x, y, w, h, 8, TFT_YELLOW);
//     tft.drawRoundRect(x + 1, y + 1, w - 2, h - 2, 7, TFT_YELLOW);
//   } else {
//     tft.fillRoundRect(x, y, w, h, 8, TFT_DARKGREY);
//     tft.drawRoundRect(x, y, w, h, 8, TFT_WHITE);
//   }
  
//   tft.setTextSize(2);
//   if (selected) {
//     tft.setTextColor(TFT_WHITE);
//   } else {
//     tft.setTextColor(TFT_LIGHTGREY);
//   }
  
//   int16_t textWidth = text.length() * 12;
//   int16_t textX = x + (w - textWidth) / 2;
//   int16_t textY = y + (h - 16) / 2;
  
//   tft.setCursor(textX, textY);
//   tft.println(text);
// }

// void drawMenu() {
//   tft.fillScreen(TFT_BLACK);
  
//   tft.setTextSize(3);
//   tft.setTextColor(TFT_YELLOW);
//   tft.setCursor(110, 30);
//   tft.println("SUNGKA");
  
//   tft.fillRect(100, 65, 120, 2, TFT_YELLOW);
  
//   drawButton(100, 80, 120, 35, "PvP", menuSelection == 0);
//   drawButton(100, 125, 120, 35, "PvAI", menuSelection == 1);
//   drawButton(100, 170, 120, 35, "Leaderboard", menuSelection == 2);
  
//   tft.setTextColor(TFT_CYAN);
//   tft.setTextSize(1);
//   tft.setCursor(80, 220);
//   tft.println("< LEFT | SELECT | RIGHT >");
// }

// void drawGameBoard() {
//   tft.fillScreen(TFT_DARKGREY);
  
//   // Draw P2 pits (top row) - indices 15 to 9
//   for (int i = 0; i < 7; i++) {
//     int pitIndex = 15 - i;
//     int x = 80 + (i * 30);
//     int y = 40;
//     bool isActive = (p2State == ANIMATING && p2CurrentPos == pitIndex);
//     bool isSelected = ((p2State == SELECTING || p2State == READY) && p2Selected == pitIndex);
//     drawPit(x, y, 12, pits[pitIndex], isActive, isSelected);
//   }
  
//   // Draw P1 pits (bottom row) - indices 1 to 7
//   for (int i = 0; i < 7; i++) {
//     int pitIndex = i + 1;
//     int x = 80 + (i * 30);
//     int y = 140;
//     bool isActive = (p1State == ANIMATING && p1CurrentPos == pitIndex);
//     bool isSelected = ((p1State == SELECTING || p1State == READY) && p1Selected == pitIndex);
//     drawPit(x, y, 12, pits[pitIndex], isActive, isSelected);
//   }
  
//   // Draw P1 store (left) - index 0
//   bool p1StoreActive = (p1State == ANIMATING && p1CurrentPos == 0);
//   drawStore(30, 90, 20, pits[0], p1StoreActive, "P1");
  
//   // Draw P2 store (right) - index 8
//   bool p2StoreActive = (p2State == ANIMATING && p2CurrentPos == 8);
//   drawStore(290, 90, 20, pits[8], p2StoreActive, "P2");
  
//   // Display instructions
//   tft.setTextSize(1);
//   tft.setTextColor(TFT_WHITE);
  
//   String p1Status = (p1State == SELECTING) ? "P1: Selecting" : (p1State == READY) ? "P1: Ready" : (p1State == ANIMATING) ? "P1: Moving" : "P1: Waiting";
//   String p2Status = (p2State == SELECTING) ? "P2: Selecting" : (p2State == READY) ? "P2: Ready" : (p2State == ANIMATING) ? "P2: Moving" : "P2: Waiting";
  
//   tft.setCursor(10, 220);
//   tft.print(p1Status);
//   tft.setCursor(190, 220);
//   tft.print(p2Status);
// }

// void drawPit(int x, int y, int radius, int seeds, bool active, bool selected) {
//   uint16_t color = TFT_WHITE;
//   uint16_t borderColor = TFT_BLACK;
  
//   if (active) {
//     color = TFT_YELLOW;
//     borderColor = TFT_RED;
//   } else if (selected) {
//     color = TFT_LIGHTGREY;
//     borderColor = TFT_GREEN;
//   }
  
//   tft.fillCircle(x, y, radius, color);
//   tft.drawCircle(x, y, radius, borderColor);
//   tft.drawCircle(x, y, radius + 1, borderColor);
  
//   tft.setTextSize(1);
//   tft.setTextColor(TFT_BLACK);
  
//   int textX = (seeds >= 10) ? x - 6 : x - 3;
//   int textY = y - 4;
  
//   tft.setCursor(textX, textY);
//   tft.print(seeds);
// }

// void drawStore(int x, int y, int radius, int seeds, bool active, String label) {
//   uint16_t color = TFT_CYAN;
//   uint16_t borderColor = TFT_BLUE;
  
//   if (active) {
//     color = TFT_YELLOW;
//     borderColor = TFT_RED;
//   }
  
//   tft.fillCircle(x, y, radius, color);
//   tft.drawCircle(x, y, radius, borderColor);
//   tft.drawCircle(x, y, radius + 1, borderColor);
  
//   tft.setTextSize(1);
//   tft.setTextColor(TFT_BLACK);
  
//   int textX = (seeds >= 10) ? x - 6 : x - 3;
//   int textY = y - 4;
  
//   tft.setCursor(textX, textY);
//   tft.print(seeds);
  
//   // Draw label
//   tft.setTextColor(TFT_WHITE);
//   tft.setCursor(x - 6, y + radius + 5);
//   tft.print(label);
// }

// void drawPvAIScreen() {
//   tft.fillScreen(TFT_MAROON);
  
//   tft.fillRoundRect(60, 60, 200, 100, 15, TFT_RED);
//   tft.drawRoundRect(60, 60, 200, 100, 15, TFT_ORANGE);
  
//   tft.setTextColor(TFT_WHITE);
//   tft.setTextSize(3);
//   tft.setCursor(80, 85);
//   tft.println("Player vs AI");
  
//   drawButton(100, 180, 120, 40, "BACK", false);
// }

// void drawLeaderboardScreen() {
//   tft.fillScreen(TFT_DARKGREEN);
  
//   tft.fillRoundRect(60, 60, 200, 100, 15, TFT_GREEN);
//   tft.drawRoundRect(60, 60, 200, 100, 15, TFT_YELLOW);
  
//   tft.setTextColor(TFT_BLACK);
//   tft.setTextSize(3);
//   tft.setCursor(70, 95);
//   tft.println("LEADERBOARD");
  
//   drawButton(100, 180, 120, 40, "BACK", false);
// }
// #include <Wire.h>
// #include <PCF8575.h>

// // Initialize PCF8575 with default address 0x20
// PCF8575 expander(0x20);

// // Button pins on PCF8575
// #define BTN_P1_LEFT   12 //14
// #define BTN_P1_SELECT 13 //15
// #define BTN_P1_RIGHT  3
// #define BTN_P2_LEFT   14  // 16
// #define BTN_P2_SELECT 7 // 12
// #define BTN_P2_RIGHT  15


//  // 17

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


// // // THE FIRST CODE ABOVE IS USED FOR DETECTING BUTTONS LANG

// #include <Arduino.h>
// #include <Wire.h> // Required for I2C communication (PCF8575)
// #define USER_SETUP_LOADED
// #include "User_Setup.h"
// #include <SPI.h>
// #include <TFT_eSPI.h>

// // devkitc4 + expander
// const int I2C_SDA = 21; 
// const int I2C_SCL = 22; 

// #include "Structures.h" 
// #include "InputHandler.h"
// #include "GameDisplay.h" 
// #include "GameController.h"

// // global instances
// GameDisplay display; 
// InputHandler inputHandler;
// GameController controller;
// TFT_eSPI tft = TFT_eSPI();
// void setup() {
//     Serial.begin(115200);    
//     Serial.println("--- Sungka Game Initialization (ESP32) ---");

//     // init i2c comms
//     Wire.begin(I2C_SDA, I2C_SCL); 

//     inputHandler.init();
//     display.init(); 


//     controller.init(&display, &inputHandler); 
//     controller.startGame(); 
// }

// void loop() {
//     ButtonEvent event = inputHandler.checkButtons();
    
//     if (event != NONE) {
//         controller.handleButtonPress(event);
//     }
    
//     controller.update();
// }


