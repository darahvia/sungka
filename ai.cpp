#include <Arduino.h>
#include "ai.h"

AI::AI() {
  randomSeed(analogRead(0));
}

int AI::getMove(Game* game) {
  int validMoves[7];
  int count = 0;
  
  for (int i = 8; i <= 14; i++) {
    if (isValidMove(game, i)) {
      validMoves[count++] = i;
    }
  }
  
  if (count == 0) return -1;
  
  int randomIndex = random(count);
  return validMoves[randomIndex];
}

bool AI::isValidMove(Game* game, int pit) {
  if (pit < 8 || pit > 14) return false;
  
  if (game->pits[pit] == 0) return false;
  
  return true;
}