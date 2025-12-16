#include <Arduino.h>
#include "Display.h"


Display::Display(TFT_eSPI* display) {
  tft = display;
}

void Display::drawButton(int x, int y, int w, int h, String text, bool selected) {
  tft->fillRoundRect(x + 3, y + 3, w, h, 8, TFT_DARKGREY);
  
  uint16_t fillCol = selected ? TFT_ORANGE : TFT_DARKGREY;
  uint16_t borderCol = selected ? TFT_YELLOW : TFT_WHITE;
  uint16_t textCol = selected ? TFT_WHITE : TFT_LIGHTGREY;
  
  tft->fillRoundRect(x, y, w, h, 8, fillCol);
  tft->drawRoundRect(x, y, w, h, 8, borderCol);
  
  tft->setTextSize(2);
  tft->setTextColor(textCol);
  
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

void Display::drawBackground() {
  if (bg_image != NULL) {
    tft->fillScreen(0x2165); 
  } else {
    tft->fillScreen(0x2165); 
  }
}

void Display::drawGameBoard(Game* game) {
  drawBackground();
  
  tft->setRotation(0); 
  
  for (int i = 0; i < 7; i++) {
    int pitIndex = i + 1;
    int x = 80 + (i * 30); 
    int y = 170; 
    
    bool isSelected = ((game->p1State == SELECTING || game->p1State == READY) && game->p1Selected == pitIndex);
    drawPit(x, y, 12, game->pits[pitIndex], isSelected, pitIndex, game);
  }

  drawStore(30, 120, 20, game->pits[0], "P1", 0, game);

  tft->setTextSize(2);
  tft->setTextColor(TFT_YELLOW);
  String p1Msg = "WAITING";
  if (game->p1State == SELECTING) p1Msg = "YOUR TURN";
  else if (game->p1State == ANIMATING) p1Msg = "MOVING...";
  
  int p1MsgX = (320 - (p1Msg.length() * 12)) / 2;
  tft->setCursor(p1MsgX, 215);
  tft->print(p1Msg);

  
  tft->setRotation(2); 

  for (int i = 0; i < 7; i++) {
    int pitIndex = 8 + i;

    int x = 240 - (i * 30);
    int y = 170; 
    bool isSelected = ((game->p2State == SELECTING || game->p2State == READY) && game->p2Selected == pitIndex);
    drawPit(x, y, 12, game->pits[pitIndex], isSelected, pitIndex, game);
  }

  drawStore(30, 120, 20, game->pits[15], "P2", 15, game);


  tft->setTextSize(2);
  tft->setTextColor(TFT_CYAN);
  String p2Msg = "WAITING";
  if (game->p2State == SELECTING) p2Msg = "YOUR TURN";
  else if (game->p2State == ANIMATING) p2Msg = "MOVING...";
  
  int p2MsgX = (320 - (p2Msg.length() * 12)) / 2;
  tft->setCursor(p2MsgX, 215); 
  tft->print(p2Msg);

 
  tft->setRotation(0);
}

void Display::drawPvAIGameBoard(Game* game) {

  drawBackground();
  


  for (int i = 0; i < 7; i++) {
    int pitIndex = 8 + i;
    int x = 80 + (i * 30);
    int y = 70; 
    bool isSelected = ((game->p2State == SELECTING || game->p2State == READY) && game->p2Selected == pitIndex);
    drawPit(x, y, 12, game->pits[pitIndex], isSelected, pitIndex, game);
  }
  

  for (int i = 0; i < 7; i++) {
    int pitIndex = i + 1;
    int x = 80 + (i * 30);
    int y = 170; 
    bool isSelected = ((game->p1State == SELECTING || game->p1State == READY) && game->p1Selected == pitIndex);
    drawPit(x, y, 12, game->pits[pitIndex], isSelected, pitIndex, game);
  }
  

  drawStore(30, 120, 20, game->pits[0], "YOU", 0, game);
  drawStore(290, 120, 20, game->pits[15], "AI", 15, game);
  

  tft->setTextSize(1);
  tft->setTextColor(TFT_WHITE);
  
  tft->setCursor(10, 220);
  tft->print(game->p1State == SELECTING ? "YOUR TURN" : "WAITING");
  
  tft->setCursor(10, 10);
  tft->print("AI: ");
  tft->print(game->p2State == SELECTING ? "THINKING..." : "WAITING");
  
  // Rule Msg
  tft->setTextColor(TFT_YELLOW);
  int msgLen = game->lastRuleMsg.length() * 6;
  int msgX = max(0, (320 - msgLen) / 2);
  tft->setCursor(msgX, 195); 
  tft->print(game->lastRuleMsg);
}

void Display::drawPit(int x, int y, int radius, int seeds, bool selected, int pitIndex, Game* game) {
  // Selection
  if (selected) {
    tft->drawCircle(x, y, radius + 4, TFT_GREEN);
    tft->drawCircle(x, y, radius + 5, TFT_GREEN);
  }
  
  //  Active Hand Indicator
  bool isP1Active = (game->p1State == ANIMATING && game->p1CurrentPos == pitIndex);
  bool isP2Active = (game->p2State == ANIMATING && game->p2CurrentPos == pitIndex);

  if (isP1Active || isP2Active) {
    uint16_t color = isP1Active ? TFT_YELLOW : TFT_RED;
    int handCount = isP1Active ? game->p1SeedsInHand : game->p2SeedsInHand;
    
    tft->drawCircle(x, y, radius + 2, color);
    tft->drawCircle(x, y, radius + 3, color);
    
    // Draw Hand Count Floating
    tft->setTextColor(color, TFT_BLACK);
    tft->setTextSize(1);
    tft->setCursor(x - 3, y - radius - 12); 
    tft->print(handCount);
  }

  //  Pit Body
  tft->fillCircle(x, y, radius, TFT_WHITE);
  tft->drawCircle(x, y, radius, TFT_BLACK);
  
  //  Seeds
  tft->setTextSize(1);
  tft->setTextColor(TFT_BLACK);
  int textX = (seeds >= 10) ? x - 6 : x - 3;
  tft->setCursor(textX, y - 4);
  tft->print(seeds);
  
  //  Index
  tft->setTextColor(TFT_RED); 
  int indexTextX = (pitIndex >= 10) ? x - 6 : x - 3;
  tft->setCursor(indexTextX, y + radius + 5);
  tft->print(pitIndex);
}

void Display::drawStore(int x, int y, int radius, int seeds, String label, int storeIndex, Game* game) {
  // Active Hand Indicator
  bool isP1Active = (game->p1State == ANIMATING && game->p1CurrentPos == storeIndex);
  bool isP2Active = (game->p2State == ANIMATING && game->p2CurrentPos == storeIndex);

  if (isP1Active || isP2Active) {
    uint16_t color = isP1Active ? TFT_YELLOW : TFT_RED;
    int handCount = isP1Active ? game->p1SeedsInHand : game->p2SeedsInHand;
    
    tft->drawCircle(x, y, radius + 2, color);
    tft->drawCircle(x, y, radius + 3, color);
    
    tft->setTextColor(color, TFT_BLACK);
    tft->setTextSize(1);
    tft->setCursor(x - 3, y - radius - 12);
    tft->print(handCount);
  }
  
  // Store Body
  tft->fillCircle(x, y, radius, TFT_CYAN);
  tft->drawCircle(x, y, radius, TFT_BLUE);
  
  // Seeds
  tft->setTextSize(1);
  tft->setTextColor(TFT_BLACK);
  int textX = (seeds >= 10) ? x - 6 : x - 3;
  tft->setCursor(textX, y - 4);
  tft->print(seeds);
  
  // Label
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


void Display::drawRulesScreen(int page) {
  tft->fillScreen(TFT_BLACK);
  
  tft->setTextSize(2);
  tft->setTextColor(TFT_YELLOW);
  tft->setCursor(10, 10);
  tft->print("RULES ("); tft->print(page); tft->print("/3)");
  
  tft->setTextSize(1);
  tft->setTextColor(TFT_WHITE);
  int y = 40;
  int x = 10;
  int indent = 15;

  if (page == 1) {
    tft->setTextColor(TFT_CYAN); tft->setCursor(x, y); tft->println("1. OBJECTIVE"); y+=10;
    tft->setTextColor(TFT_WHITE); tft->setCursor(indent, y); tft->println("- Collect most seeds in store."); y+=15;
    
    tft->setTextColor(TFT_CYAN); tft->setCursor(x, y); tft->println("2. START (Round 1)"); y+=10;
    tft->setTextColor(TFT_WHITE); tft->setCursor(indent, y); tft->println("- Simultaneous play."); y+=10;
    tft->setCursor(indent, y); tft->println("- P1(4) and P2(11) default."); y+=15;

    tft->setTextColor(TFT_CYAN); tft->setCursor(x, y); tft->println("3. MOVES"); y+=10;
    tft->setTextColor(TFT_WHITE); tft->setCursor(indent, y); tft->println("- Pick all seeds in house."); y+=10;
    tft->setCursor(indent, y); tft->println("- Sow 1-by-1 Counter-Clockwise."); y+=10;
    tft->setCursor(indent, y); tft->println("- Skip opponent's store."); y+=15;
  }
  else if (page == 2) {
    tft->setTextColor(TFT_CYAN); tft->setCursor(x, y); tft->println("4. CHAINING"); y+=10;
    tft->setTextColor(TFT_WHITE); tft->setCursor(indent, y); tft->println("- Last seed in non-empty pit?"); y+=10;
    tft->setCursor(indent, y); tft->println("- Pick up & continue sowing."); y+=15;

    tft->setTextColor(TFT_CYAN); tft->setCursor(x, y); tft->println("5. EXTRA TURN"); y+=10;
    tft->setTextColor(TFT_WHITE); tft->setCursor(indent, y); tft->println("- Land in OWN Store?"); y+=10;
    tft->setCursor(indent, y); tft->println("- Go again (except Round 1)."); y+=15;

    tft->setTextColor(TFT_CYAN); tft->setCursor(x, y); tft->println("6. CAPTURE"); y+=10;
    tft->setTextColor(TFT_WHITE); tft->setCursor(indent, y); tft->println("- Land empty pit (Own Side)?"); y+=10;
    tft->setCursor(indent, y); tft->println("- Capture own + opposite seeds."); y+=10;
    tft->setCursor(indent, y); tft->println("- Goes to your store."); y+=15;
  }
  else {
    tft->setTextColor(TFT_CYAN); tft->setCursor(x, y); tft->println("7. END TURN"); y+=10;
    tft->setTextColor(TFT_WHITE); tft->setCursor(indent, y); tft->println("- Land empty pit (Opp Side)."); y+=15;

    tft->setTextColor(TFT_CYAN); tft->setCursor(x, y); tft->println("8/9. GAME END"); y+=10;
    tft->setTextColor(TFT_WHITE); tft->setCursor(indent, y); tft->println("- One side empty & no moves."); y+=10;
    tft->setCursor(indent, y); tft->println("- Remaining seeds collected."); y+=15;

    tft->setTextColor(TFT_CYAN); tft->setCursor(x, y); tft->println("10. WINNER"); y+=10;
    tft->setTextColor(TFT_WHITE); tft->setCursor(indent, y); tft->println("- Most seeds wins."); y+=15;
  }

  if (page > 1) drawButton(10, 200, 90, 30, "PREV", false);
  else drawButton(10, 200, 90, 30, "BACK", false);

  if (page < 3) drawButton(220, 200, 90, 30, "NEXT", false);
  else drawButton(220, 200, 90, 30, "MENU", false);
}