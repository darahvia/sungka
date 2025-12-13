#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>
#include <PCF8575.h>
#include "Constants.h"
#include "Game.h"

class Input {
private:
  PCF8575* expander;
  bool lastState[16];
  
  void handleP1Button(int btn, Game* game);
  void handleP2Button(int btn, Game* game);

public:
  Input(PCF8575* exp);
  void begin();
  bool checkButton(int btn);
  void handleMenuButton(int btn, int* menuSelection, Screen* currentScreen);
  void handleGameButton(int btn, Game* game);
  void handleBackButton(int btn, Screen* currentScreen, int* menuSelection);
};

#endif