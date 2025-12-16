#include <Arduino.h>
#include "Input.h"

Input::Input(PCF8575* exp) {
  expander = exp;
  for (int i = 0; i < 16; i++) {
    lastState[i] = false;
  }
}

void Input::begin() {
  for (int i = 1; i <= 15; i++) {
    expander->write(i, LOW);
  }
}

bool Input::checkButton(int btn) {
  bool currentState = expander->read(btn);
  
  if (currentState == HIGH && lastState[btn] == LOW) {
    lastState[btn] = currentState;
    return true;
  }
  
  lastState[btn] = currentState;
  return false;
}

void Input::handleMenuButton(int btn, int* menuSelection, Screen* currentScreen) {
  if (btn == BTN_P1_LEFT || btn == BTN_P2_LEFT) {
    (*menuSelection)--;
    if (*menuSelection < 0) *menuSelection = 2;
  }
  else if (btn == BTN_P1_RIGHT || btn == BTN_P2_RIGHT) {
    (*menuSelection)++;
    if (*menuSelection > 2) *menuSelection = 0;
  }
  else if (btn == BTN_P1_SELECT || btn == BTN_P2_SELECT) {
    if (*menuSelection == 0) {
      *currentScreen = PVP;
    }
    else if (*menuSelection == 1) {
      *currentScreen = PVA;
    }
    else if (*menuSelection == 2) {
      *currentScreen = RULES;
    }
  }
}

void Input::handleGameButton(int btn, Game* game) {
  if (btn == BTN_P1_LEFT || btn == BTN_P1_RIGHT || btn == BTN_P1_SELECT) {
    handleP1Button(btn, game);
  }
  else if (btn == BTN_P2_LEFT || btn == BTN_P2_RIGHT || btn == BTN_P2_SELECT) {
    handleP2Button(btn, game);
  }
}

void Input::handleP1Button(int btn, Game* game) {
  if (game->p1State == SELECTING) {
    if (btn == BTN_P1_LEFT) {
      game->p1Selected--;
      if (game->p1Selected < 1) game->p1Selected = 7;
    }
    else if (btn == BTN_P1_RIGHT) {
      game->p1Selected++;
      if (game->p1Selected > 7) game->p1Selected = 1;
    }
    else if (btn == BTN_P1_SELECT && game->p1Selected != -1) {
      if (game->firstMove) {
        game->p1State = READY;
        if (game->p2State == READY) {
          game->startFirstMove();
        }
      } else {
        game->startP1Move();
      }
    }
  }
}

void Input::handleP2Button(int btn, Game* game) {
  if (game->p2State == SELECTING) {
    if (btn == BTN_P2_LEFT) {
      game->p2Selected--;
      if (game->p2Selected < 8) game->p2Selected = 14;
    }
    else if (btn == BTN_P2_RIGHT) {
      game->p2Selected++;
      if (game->p2Selected > 14) game->p2Selected = 8;
    }
    else if (btn == BTN_P2_SELECT && game->p2Selected != -1) {
      if (game->firstMove) {
        game->p2State = READY;
        if (game->p1State == READY) {
          game->startFirstMove();
        }
      } else {
        game->startP2Move();
      }
    }
  }
}

void Input::handleBackButton(int btn, Screen* currentScreen, int* menuSelection) {
  if (btn == BTN_P1_SELECT || btn == BTN_P2_SELECT) {
    *currentScreen = MENU;
    *menuSelection = 0;
  }
}