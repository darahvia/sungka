// GameDisplay.h

#ifndef GAME_DISPLAY_H
#define GAME_DISPLAY_H

#include <Arduino.h>
// #include <TFT_eSPI.h>

#include "Structures.h"

class GameDisplay {
public:
    void init();
    void clearScreen();
    
    // menu
    void drawMenu(MenuOption selection);
    
    // gameboard
    void drawSungkaBoard(const GameState* state, int selectedPit);

private:
    // TFT_eSPI tft; 
};

#endif