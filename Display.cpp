#include <Arduino.h>
#include "Display.h"

Display::Display(TFT_eSPI* display) {
  tft = display;
}

void Display::drawButton(int x, int y, int w, int h, String text, bool selected) {
  tft->fillRoundRect(x + 3, y + 3, w, h, 8, TFT_DARKGREY);
  
  if (selected) {
    tft->fillRoundRect(x, y, w, h, 8, TFT_ORANGE);
    tft->drawRoundRect(x, y, w, h, 8, TFT_YELLOW);
    tft->drawRoundRect(x + 1, y + 1, w - 2, h - 2, 7, TFT_YELLOW);
  } else {
    tft->fillRoundRect(x, y, w, h, 8, TFT_DARKGREY);
    tft->drawRoundRect(x, y, w, h, 8, TFT_WHITE);
  }
  
  tft->setTextSize(2);
  if (selected) {
    tft->setTextColor(TFT_WHITE);
  } else {
    tft->setTextColor(TFT_LIGHTGREY);
  }
  
  int16_t textWidth = text.length() * 12;
  int16_t textX = x + (w - textWidth) / 2;
  int16_t textY = y + (h - 16) / 2;
  
  tft->setCursor(textX, textY);
  tft->println(text);
}

void Display::drawMenu(int selection) {
  tft->fillScreen(TFT_BLACK);
  
  tft->setTextSize(3);
  tft->setTextColor(TFT_YELLOW);
  tft->setCursor(110, 30);
  tft->println("SUNGKA");
  
  tft->fillRect(100, 65, 120, 2, TFT_YELLOW);
  
  drawButton(100, 80, 120, 35, "PvP", selection == 0);
  drawButton(100, 125, 120, 35, "PvAI", selection == 1);
  drawButton(100, 170, 120, 35, "Rules", selection == 2);
  
  tft->setTextColor(TFT_CYAN);
  tft->setTextSize(1);
  tft->setCursor(80, 220);
  tft->println("< LEFT | SELECT | RIGHT >");
}

void Display::drawGameBoard(Game* game) {
  tft->fillScreen(TFT_DARKGREY);
  
  // Draw P2 pits
  for (int i = 0; i < 7; i++) {
    int pitIndex = 8 + i;
    int x = 80 + (i * 30);
    int y = 40;
    bool isSelected = ((game->p2State == SELECTING || game->p2State == READY) && game->p2Selected == pitIndex);
    drawPit(x, y, 12, game->pits[pitIndex], isSelected, pitIndex, game);
  }
  
  // Draw P1 pits
  for (int i = 0; i < 7; i++) {
    int pitIndex = i + 1;
    int x = 80 + (i * 30);
    int y = 140;
    bool isSelected = ((game->p1State == SELECTING || game->p1State == READY) && game->p1Selected == pitIndex);
    drawPit(x, y, 12, game->pits[pitIndex], isSelected, pitIndex, game);
  }
  
  // Stores
  drawStore(30, 90, 20, game->pits[0], "P1", 0, game);
  drawStore(290, 90, 20, game->pits[15], "P2", 15, game);
  
  // --- RULE / EVENT MESSAGE ---
  tft->setTextSize(1);
  tft->setTextColor(TFT_YELLOW);
  
  // Center the message
  int msgLen = game->lastRuleMsg.length() * 6;
  int msgX = max(0, (320 - msgLen) / 2);
  
  // Draw it just above the status line
  tft->setCursor(msgX, 205); 
  tft->print(game->lastRuleMsg);
  // ----------------------------

  // Display Status
  tft->setTextSize(1);
  tft->setTextColor(TFT_WHITE);
  
  String p1Status = (game->p1State == SELECTING) ? "P1: Selecting" : (game->p1State == READY) ? "P1: Ready" : (game->p1State == ANIMATING) ? "P1: Moving" : "P1: Waiting";
  String p2Status = (game->p2State == SELECTING) ? "P2: Selecting" : (game->p2State == READY) ? "P2: Ready" : (game->p2State == ANIMATING) ? "P2: Moving" : "P2: Waiting";
  
  tft->setCursor(10, 220);
  tft->print(p1Status);
  tft->setCursor(190, 220);
  tft->print(p2Status);
}

void Display::drawPvAIGameBoard(Game* game) {
  tft->fillScreen(TFT_DARKGREY);
  
  // Draw AI pits
  for (int i = 0; i < 7; i++) {
    int pitIndex = 8 + i;
    int x = 80 + (i * 30);
    int y = 40;
    bool isSelected = ((game->p2State == SELECTING || game->p2State == READY) && game->p2Selected == pitIndex);
    drawPit(x, y, 12, game->pits[pitIndex], isSelected, pitIndex, game);
  }
  
  // Draw Player pits
  for (int i = 0; i < 7; i++) {
    int pitIndex = i + 1;
    int x = 80 + (i * 30);
    int y = 140;
    bool isSelected = ((game->p1State == SELECTING || game->p1State == READY) && game->p1Selected == pitIndex);
    drawPit(x, y, 12, game->pits[pitIndex], isSelected, pitIndex, game);
  }
  
  // Stores
  drawStore(30, 90, 20, game->pits[0], "YOU", 0, game);
  drawStore(290, 90, 20, game->pits[15], "AI", 15, game);
  
  // --- RULE / EVENT MESSAGE ---
  tft->setTextSize(1);
  tft->setTextColor(TFT_YELLOW);
  int msgLen = game->lastRuleMsg.length() * 6;
  int msgX = max(0, (320 - msgLen) / 2);
  tft->setCursor(msgX, 205); 
  tft->print(game->lastRuleMsg);
  // ----------------------------
  
  // Status
  tft->setTextSize(1);
  tft->setTextColor(TFT_WHITE);
  
  String p1Status = (game->p1State == SELECTING) ? "Your Turn" : 
                    (game->p1State == READY) ? "Ready" : 
                    (game->p1State == ANIMATING) ? "Moving..." : "Wait";
  String p2Status = (game->p2State == SELECTING) ? "AI Thinking" : 
                    (game->p2State == READY) ? "AI Ready" : 
                    (game->p2State == ANIMATING) ? "AI Moving" : "AI Wait";
  
  tft->setCursor(10, 220);
  tft->print(p1Status);
  tft->setCursor(180, 220);
  tft->print(p2Status);
}

// ... [Keep drawPit, drawStore, drawPvAIScreen, drawLeaderboardScreen same as before] ...
void Display::drawPit(int x, int y, int radius, int seeds, bool selected, int pitIndex, Game* game) {
  // 1. Draw Selection Indicator (Green)
  if (selected) {
    tft->drawCircle(x, y, radius + 4, TFT_GREEN);
    tft->drawCircle(x, y, radius + 5, TFT_GREEN);
  }
  
  // 2. Draw Active Movement Indicator (Halo + Seeds in Hand)
  
  // --- Player 1 Logic (Yellow) ---
  if (game->p1State == ANIMATING && game->p1CurrentPos == pitIndex) {
    // Halo
    tft->drawCircle(x, y, radius + 2, TFT_YELLOW);
    tft->drawCircle(x, y, radius + 3, TFT_YELLOW);
    
    // Show Hand Count ABOVE the pit
    tft->setTextColor(TFT_YELLOW, TFT_DARKGREY);
    tft->setTextSize(1);
    tft->setCursor(x - 3, y - radius - 12); 
    tft->print(game->p1SeedsInHand);
  }
  
  // --- Player 2 Logic (Red) ---
  if (game->p2State == ANIMATING && game->p2CurrentPos == pitIndex) {
    // Halo
    tft->drawCircle(x, y, radius + 2, TFT_RED);
    tft->drawCircle(x, y, radius + 3, TFT_RED);
    
    // Show Hand Count
    tft->setTextColor(TFT_RED, TFT_DARKGREY);
    tft->setTextSize(1);
    int yOffset = (game->p1State == ANIMATING && game->p1CurrentPos == pitIndex) ? 22 : 12;
    tft->setCursor(x - 3, y - radius - yOffset);
    tft->print(game->p2SeedsInHand);
  }

  // 3. Draw Pit Body
  tft->fillCircle(x, y, radius, TFT_WHITE);
  tft->drawCircle(x, y, radius, TFT_BLACK);
  
  // 4. Draw Seeds Count inside pit
  tft->setTextSize(1);
  tft->setTextColor(TFT_BLACK);
  
  int textX = (seeds >= 10) ? x - 6 : x - 3;
  int textY = y - 4;
  
  tft->setCursor(textX, textY);
  tft->print(seeds);
  
  // 5. Draw Pit Index (Red text below)
  tft->setTextColor(TFT_RED);
  int indexTextX = (pitIndex >= 10) ? x - 6 : x - 3;
  int indexTextY = y + radius + 5;
  tft->setCursor(indexTextX, indexTextY);
  tft->print(pitIndex);
}

void Display::drawStore(int x, int y, int radius, int seeds, String label, int storeIndex, Game* game) {
  // 1. Draw Active Movement Indicator
  // --- Player 1 (Yellow) ---
  if (game->p1State == ANIMATING && game->p1CurrentPos == storeIndex) {
    tft->drawCircle(x, y, radius + 2, TFT_YELLOW);
    tft->drawCircle(x, y, radius + 3, TFT_YELLOW);
    
    // Show Hand Count
    tft->setTextColor(TFT_YELLOW, TFT_DARKGREY);
    tft->setTextSize(1);
    tft->setCursor(x - 3, y - radius - 12);
    tft->print(game->p1SeedsInHand);
  }
  
  // --- Player 2 (Red) ---
  if (game->p2State == ANIMATING && game->p2CurrentPos == storeIndex) {
    tft->drawCircle(x, y, radius + 2, TFT_RED);
    tft->drawCircle(x, y, radius + 3, TFT_RED);
    
    // Show Hand Count
    tft->setTextColor(TFT_RED, TFT_DARKGREY);
    tft->setTextSize(1);
    int yOffset = (game->p1State == ANIMATING && game->p1CurrentPos == storeIndex) ? 22 : 12;
    tft->setCursor(x - 3, y - radius - yOffset);
    tft->print(game->p2SeedsInHand);
  }
  
  // 2. Draw Store Body
  tft->fillCircle(x, y, radius, TFT_CYAN);
  tft->drawCircle(x, y, radius, TFT_BLUE);
  
  // 3. Draw Seeds Count
  tft->setTextSize(1);
  tft->setTextColor(TFT_BLACK);
  
  int textX = (seeds >= 10) ? x - 6 : x - 3;
  int textY = y - 4;
  
  tft->setCursor(textX, textY);
  tft->print(seeds);
  
  // 4. Draw Label
  tft->setTextColor(TFT_WHITE);
  tft->setCursor(x - 6, y + radius + 6);
  tft->print(label);
}

void Display::drawPvAIScreen() {
  tft->fillScreen(TFT_MAROON);
  tft->fillRoundRect(60, 60, 200, 100, 15, TFT_RED);
  tft->drawRoundRect(60, 60, 200, 100, 15, TFT_ORANGE);
  
  tft->setTextColor(TFT_WHITE);
  tft->setTextSize(3);
  tft->setCursor(80, 85);
  tft->println("Player vs AI");
  
  drawButton(100, 180, 120, 40, "BACK", false);
}

void Display::drawRulesScreen() {
  tft->fillScreen(TFT_DARKGREEN);
  tft->fillRoundRect(60, 60, 200, 100, 15, TFT_GREEN);
  tft->drawRoundRect(60, 60, 200, 100, 15, TFT_YELLOW);
  
  tft->setTextColor(TFT_BLACK);
  tft->setTextSize(3);
  tft->setCursor(70, 95);
  tft->println("LEADERBOARD");
  
  drawButton(100, 180, 120, 40, "BACK", false);
}