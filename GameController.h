// GameController.h

#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <Arduino.h>
#include "Structures.h"
#include "GameDisplay.h"
#include "InputHandler.h"

class GameController {
public:
    void init(GameDisplay* disp, InputHandler* input);
    void startGame();
    void update();
    void handleButtonPress(ButtonEvent event);

private:
    GameDisplay* display;
    InputHandler* input;
    
    GameState gameState;
    MenuOption currentMenuSelection = MENU_PVP;

    // state handlers
    void runMenuState(ButtonEvent event);
    void runPvPState(ButtonEvent event);
    
    // menu logic
    void updateMenuDisplay(); // render menu
    void navigateMenu(ButtonEvent event);
};

#endif