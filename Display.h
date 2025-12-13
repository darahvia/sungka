#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Constants.h"
#include "Game.h"

class Display {
private:
  TFT_eSPI* tft;
  
  // Updated helpers to accept Game pointer
  void drawButton(int x, int y, int w, int h, String text, bool selected);
  void drawPit(int x, int y, int radius, int seeds, bool selected, int pitIndex, Game* game);
  void drawStore(int x, int y, int radius, int seeds, String label, int storeIndex, Game* game);

public:
  Display(TFT_eSPI* display);
  void drawMenu(int selection);
  void drawGameBoard(Game* game);
  void drawPvAIScreen();
  void drawPvAIGameBoard(Game* game);
  void drawLeaderboardScreen();
};

#endif