// GameController.cpp

#include "GameController.h"

void GameController::init(GameDisplay* disp, InputHandler* input) {
    this->display = disp;
    this->input = input;
    gameState.currentScreen = MENU;
    Serial.println("-> GameController Initialized.");
}

void GameController::startGame() {
    // initial setup when game starts
    Serial.println("--- Starting Game: Menu State ---");
    //force the display to render the initial menu state (currently stubbed)
    updateMenuDisplay(); 
}

// MAIN LOOP FUNCTIONS

void GameController::update() {
    // every loop iteration, regardless of input
    // mainly used for ongoing animations or AI processing
}

void GameController::handleButtonPress(ButtonEvent event) {
    // delegate handling based on the current screen state
    switch (gameState.currentScreen) {
        case MENU:
            runMenuState(event);
            break;
        case PLAYER_VS_PLAYER:
            runPvPState(event);
            break;
        // TODO: add cases for PLAYER_VS_AI, LEADERBOARD, etc.
        default:
            break;
    }
}

//STATES

void GameController::runMenuState(ButtonEvent event) {
    switch (event) {
        case P1_LEFT:
        case P1_RIGHT:
            navigateMenu(event);
            break;
        case P1_SELECT:
            if (currentMenuSelection == MENU_PVP) {
                gameState.currentScreen = PLAYER_VS_PLAYER;
                Serial.println("\n--- State Change: Entered PLAYER_VS_PLAYER ---");
                Serial.println("(Press P2_SELECT to return to menu)");
            } else if (currentMenuSelection == MENU_P_AI) {
                gameState.currentScreen = PLAYER_VS_AI;
                Serial.println("\n--- State Change: Entered PLAYER_VS_AI (Not implemented) ---");
            } else if (currentMenuSelection == MENU_LEADERBOARD) {
                gameState.currentScreen = LEADERBOARD;
                Serial.println("\n--- State Change: Entered LEADERBOARD (Not implemented) ---");
            }
            break;
        default:
            break;
    }
}

void GameController::runPvPState(ButtonEvent event) {
    // TODO: sungka turn logic
    Serial.print("PvP: Received event: ");
    Serial.println(buttonEventToString(event));

    // example: use Player 1 buttons if it's Player 1's turn
    // if (gameState.currentPlayer == 1) { ... }
    
    // DEBUG: allow P2 select to return to menu for easy testing
    if (event == P2_SELECT) {
        gameState.currentScreen = MENU;
        updateMenuDisplay();
        Serial.println("\n--- State Change: Returned to MENU ---");
    }
}

// MENU MENU MENU

void GameController::navigateMenu(ButtonEvent event) {
    if (event == P1_LEFT) {
        currentMenuSelection = (MenuOption)((currentMenuSelection - 1 + MENU_COUNT) % MENU_COUNT);
    } else if (event == P1_RIGHT) {
        currentMenuSelection = (MenuOption)((currentMenuSelection + 1) % MENU_COUNT);
    }
    updateMenuDisplay();
}

void GameController::updateMenuDisplay() {
    // TODO: draw the menu on the screen 
    // display->drawMenu(currentMenuSelection); 

    // log the current menu state to serial monitor
    Serial.print("Menu Selection Updated: ");
    switch (currentMenuSelection) {
        case MENU_PVP: Serial.println("Play vs Player (PVP)"); break;
        case MENU_P_AI: Serial.println("Play vs AI"); break;
        case MENU_LEADERBOARD: Serial.println("Leaderboards"); break;
        default: break;
    }
}