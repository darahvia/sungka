#ifndef AI_H
#define AI_H

#include <Arduino.h>
#include "Game.h"

class AI {
public:
  AI();
  
  int getMove(Game* game);
  
private:
  bool isValidMove(Game* game, int pit);
};

#endif