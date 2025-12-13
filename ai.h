#ifndef AI_H
#define AI_H

#include <Arduino.h>
#include "Game.h"

class AI {
public:
  AI();
  
  // AI difficulty levels
  enum Difficulty {
    EASY,
    MEDIUM,
    HARD
  };
  
  // Get AI's next move
  int getMove(Game* game, Difficulty difficulty);
  
private:
  // Easy: Random valid move
  int getRandomMove(Game* game);
  
  // Medium: Prefer moves that give extra turns
  int getMediumMove(Game* game);
  
  // Hard: Minimax or greedy algorithm
  int getHardMove(Game* game);
  
  // Helper functions
  bool isValidMove(Game* game, int pit);
  int evaluateMove(Game* game, int pit);
  int countSeeds(Game* game, bool isP1);
};

#endif