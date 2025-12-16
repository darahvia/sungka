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