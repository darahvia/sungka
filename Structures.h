#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <Arduino.h>

// i2c address for PCF8575
const int PCF8575_ADDR = 0x20; 

// PCF8575 pin assignments
const int PIN_P1_LEFT    = 12; 
const int PIN_P1_SELECT  = 13; 
const int PIN_P1_RIGHT   = 3;  
const int PIN_P2_LEFT    = 14;  
const int PIN_P2_SELECT  = 10;  
const int PIN_P2_RIGHT   = 15; 

//ENUMS

enum ScreenState {
    MENU,
    PLAYER_VS_PLAYER,
    PLAYER_VS_AI,
    LEADERBOARD,
    GAME_OVER
};

enum ButtonEvent {
    NONE,
    P1_LEFT,
    P1_SELECT,
    P1_RIGHT,
    P2_LEFT,
    P2_SELECT,
    P2_RIGHT
};

enum MenuOption {
    MENU_PVP,
    MENU_P_AI,
    MENU_LEADERBOARD,
    MENU_COUNT
};

structs
struct GameState {
    int pits[14]; // 7 pits for P1 (index 0-6), 7 pits for P2 (index 7-13)
    int homePitP1; // Seeds in P1's home (Mancala)
    int homePitP2; // Seeds in P2's home (Mancala)
    int currentPlayer; // 1 or 2
    ScreenState currentScreen;
};


const char* buttonEventToString(ButtonEvent event);

#endif