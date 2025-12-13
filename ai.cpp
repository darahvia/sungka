#include <Arduino.h>
#include "AI.h"

AI::AI() {
  randomSeed(analogRead(0));
}

int AI::getMove(Game* game, Difficulty difficulty) {
  switch (difficulty) {
    case EASY:
      return getRandomMove(game);
    case MEDIUM:
      return getMediumMove(game);
    case HARD:
      return getHardMove(game);
    default:
      return getRandomMove(game);
  }
}

bool AI::isValidMove(Game* game, int pit) {
  // AI is P2, so valid pits are 9-15
  if (pit < 9 || pit > 15) return false;
  if (game->pits[pit] == 0) return false;
  return true;
}

int AI::getRandomMove(Game* game) {
  // Get all valid moves
  int validMoves[7];
  int count = 0;
  
  for (int i = 9; i <= 15; i++) {
    if (isValidMove(game, i)) {
      validMoves[count++] = i;
    }
  }
  
  if (count == 0) return -1;
  
  // Return random valid move
  return validMoves[random(count)];
}

int AI::getMediumMove(Game* game) {
  int bestMove = -1;
  int bestScore = -1000;
  
  // Prefer moves that land in store (extra turn)
  for (int i = 9; i <= 15; i++) {
    if (!isValidMove(game, i)) continue;
    
    int score = 0;
    int seeds = game->pits[i];
    int landingPos = i;
    
    // Simulate where seeds will land
    for (int j = 0; j < seeds; j++) {
      landingPos = game->getNextPos(landingPos, false);
    }
    
    // Extra turn bonus
    if (landingPos == 8) {
      score += 10;
    }
    
    // Prefer moves with more seeds
    score += seeds;
    
    if (score > bestScore) {
      bestScore = score;
      bestMove = i;
    }
  }
  
  return (bestMove != -1) ? bestMove : getRandomMove(game);
}

int AI::getHardMove(Game* game) {
  int bestMove = -1;
  int bestScore = -10000;
  
  for (int i = 9; i <= 15; i++) {
    if (!isValidMove(game, i)) continue;
    
    int score = evaluateMove(game, i);
    
    if (score > bestScore) {
      bestScore = score;
      bestMove = i;
    }
  }
  
  return (bestMove != -1) ? bestMove : getMediumMove(game);
}

int AI::evaluateMove(Game* game, int pit) {
  int score = 0;
  int seeds = game->pits[pit];
  int landingPos = pit;
  
  // Simulate where seeds will land
  for (int j = 0; j < seeds; j++) {
    landingPos = game->getNextPos(landingPos, false);
  }
  
  // Extra turn is very valuable
  if (landingPos == 8) {
    score += 20;
  }
  
  // Landing in empty pit (potential capture)
  if (landingPos >= 9 && landingPos <= 15 && game->pits[landingPos] == 0) {
    int oppositePit = 16 - landingPos;
    if (game->pits[oppositePit] > 0) {
      score += game->pits[oppositePit] * 2; // Capture bonus
    }
  }
  
  // Prefer moves that increase store difference
  int p1Store = game->pits[0];
  int p2Store = game->pits[8];
  score += (p2Store - p1Store);
  
  // Prefer distributing seeds widely
  score += seeds / 2;
  
  return score;
}

int AI::countSeeds(Game* game, bool isP1) {
  int total = 0;
  
  if (isP1) {
    for (int i = 1; i <= 7; i++) {
      total += game->pits[i];
    }
  } else {
    for (int i = 9; i <= 15; i++) {
      total += game->pits[i];
    }
  }
  
  return total;
}