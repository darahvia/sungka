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
  drawButton(100, 170, 120, 35, "Leaderboard", selection == 2);
  
  tft->setTextColor(TFT_CYAN);
  tft->setTextSize(1);
  tft->setCursor(80, 220);
  tft->println("< LEFT | SELECT | RIGHT >");
}

void Display::drawGameBoard(Game* game) {
  tft->fillScreen(TFT_DARKGREY);
  
  // Draw P2 pits (top row) - indices 15 to 9
  for (int i = 0; i < 7; i++) {
    int pitIndex = 15 - i;
    int x = 80 + (i * 30);
    int y = 40;
    bool isActive = (game->p2State == ANIMATING && game->p2CurrentPos == pitIndex);
    bool isSelected = ((game->p2State == SELECTING || game->p2State == READY) && game->p2Selected == pitIndex);
    drawPit(x, y, 12, game->pits[pitIndex], isActive, isSelected);
  }
  
  // Draw P1 pits (bottom row) - indices 1 to 7
  for (int i = 0; i < 7; i++) {
    int pitIndex = i + 1;
    int x = 80 + (i * 30);
    int y = 140;
    bool isActive = (game->p1State == ANIMATING && game->p1CurrentPos == pitIndex);
    bool isSelected = ((game->p1State == SELECTING || game->p1State == READY) && game->p1Selected == pitIndex);
    drawPit(x, y, 12, game->pits[pitIndex], isActive, isSelected);
  }
  
  // Draw P1 store (left) - index 0
  bool p1StoreActive = (game->p1State == ANIMATING && game->p1CurrentPos == 0);
  drawStore(30, 90, 20, game->pits[0], p1StoreActive, "P1");
  
  // Draw P2 store (right) - index 8
  bool p2StoreActive = (game->p2State == ANIMATING && game->p2CurrentPos == 8);
  drawStore(290, 90, 20, game->pits[8], p2StoreActive, "P2");
  
  // Display instructions
  tft->setTextSize(1);
  tft->setTextColor(TFT_WHITE);
  
  String p1Status = (game->p1State == SELECTING) ? "P1: Selecting" : 
                    (game->p1State == READY) ? "P1: Ready" : 
                    (game->p1State == ANIMATING) ? "P1: Moving" : "P1: Waiting";
  String p2Status = (game->p2State == SELECTING) ? "P2: Selecting" : 
                    (game->p2State == READY) ? "P2: Ready" : 
                    (game->p2State == ANIMATING) ? "P2: Moving" : "P2: Waiting";
  
  tft->setCursor(10, 220);
  tft->print(p1Status);
  tft->setCursor(190, 220);
  tft->print(p2Status);
}

void Display::drawPit(int x, int y, int radius, int seeds, bool active, bool selected) {
  uint16_t color = TFT_WHITE;
  uint16_t borderColor = TFT_BLACK;
  
  if (active) {
    color = TFT_YELLOW;
    borderColor = TFT_RED;
  } else if (selected) {
    color = TFT_LIGHTGREY;
    borderColor = TFT_GREEN;
  }
  
  tft->fillCircle(x, y, radius, color);
  tft->drawCircle(x, y, radius, borderColor);
  tft->drawCircle(x, y, radius + 1, borderColor);
  
  tft->setTextSize(1);
  tft->setTextColor(TFT_BLACK);
  
  int textX = (seeds >= 10) ? x - 6 : x - 3;
  int textY = y - 4;
  
  tft->setCursor(textX, textY);
  tft->print(seeds);
}

void Display::drawStore(int x, int y, int radius, int seeds, bool active, String label) {
  uint16_t color = TFT_CYAN;
  uint16_t borderColor = TFT_BLUE;
  
  if (active) {
    color = TFT_YELLOW;
    borderColor = TFT_RED;
  }
  
  tft->fillCircle(x, y, radius, color);
  tft->drawCircle(x, y, radius, borderColor);
  tft->drawCircle(x, y, radius + 1, borderColor);
  
  tft->setTextSize(1);
  tft->setTextColor(TFT_BLACK);
  
  int textX = (seeds >= 10) ? x - 6 : x - 3;
  int textY = y - 4;
  
  tft->setCursor(textX, textY);
  tft->print(seeds);
  
  // Draw label
  tft->setTextColor(TFT_WHITE);
  tft->setCursor(x - 6, y + radius + 5);
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

void Display::drawPvAIGameBoard(Game* game) {
  tft->fillScreen(TFT_DARKGREY);
  
  // Draw AI pits (top row) - indices 15 to 9
  for (int i = 0; i < 7; i++) {
    int pitIndex = 15 - i;
    int x = 80 + (i * 30);
    int y = 40;
    bool isActive = (game->p2State == ANIMATING && game->p2CurrentPos == pitIndex);
    bool isSelected = ((game->p2State == SELECTING || game->p2State == READY) && game->p2Selected == pitIndex);
    drawPit(x, y, 12, game->pits[pitIndex], isActive, isSelected);
  }
  
  // Draw Player pits (bottom row) - indices 1 to 7
  for (int i = 0; i < 7; i++) {
    int pitIndex = i + 1;
    int x = 80 + (i * 30);
    int y = 140;
    bool isActive = (game->p1State == ANIMATING && game->p1CurrentPos == pitIndex);
    bool isSelected = ((game->p1State == SELECTING || game->p1State == READY) && game->p1Selected == pitIndex);
    drawPit(x, y, 12, game->pits[pitIndex], isActive, isSelected);
  }
  
  // Draw Player store (left) - index 0
  bool p1StoreActive = (game->p1State == ANIMATING && game->p1CurrentPos == 0);
  drawStore(30, 90, 20, game->pits[0], p1StoreActive, "YOU");
  
  // Draw AI store (right) - index 8
  bool p2StoreActive = (game->p2State == ANIMATING && game->p2CurrentPos == 8);
  drawStore(290, 90, 20, game->pits[8], p2StoreActive, "AI");
  
  // Display status
  tft->setTextSize(1);
  tft->setTextColor(TFT_WHITE);
  
  String p1Status = (game->p1State == SELECTING) ? "Your Turn" : 
                    (game->p1State == READY) ? "Ready" : 
                    (game->p1State == ANIMATING) ? "Moving..." : "Wait";
  String p2Status = (game->p2State == SELECTING) ? "AI Thinking..." : 
                    (game->p2State == READY) ? "AI Ready" : 
                    (game->p2State == ANIMATING) ? "AI Moving..." : "AI Wait";
  
  tft->setCursor(10, 220);
  tft->print(p1Status);
  tft->setCursor(180, 220);
  tft->print(p2Status);
}

void Display::drawLeaderboardScreen() {
  tft->fillScreen(TFT_DARKGREEN);
  
  tft->fillRoundRect(60, 60, 200, 100, 15, TFT_GREEN);
  tft->drawRoundRect(60, 60, 200, 100, 15, TFT_YELLOW);
  
  tft->setTextColor(TFT_BLACK);
  tft->setTextSize(3);
  tft->setCursor(70, 95);
  tft->println("LEADERBOARD");
  
  drawButton(100, 180, 120, 40, "BACK", false);
}