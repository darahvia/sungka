#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>
#include "Game.h"

class Display {
private:
  TFT_eSPI* tft;
  
  void drawButton(int x, int y, int w, int h, String text, bool selected);
  void drawPit(int x, int y, int radius, int seeds, bool active, bool selected);
  void drawStore(int x, int y, int radius, int seeds, bool active, String label);

public:
  Display(TFT_eSPI* display);
  void drawMenu(int selection);
  void drawGameBoard(Game* game);
  void drawPvAIScreen();
  void drawPvAIGameBoard(Game* game);
  void drawLeaderboardScreen();
};

#endif