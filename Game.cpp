#include "Game.h"

Game::Game() {
  init();
}

void Game::init() {
  pits[0] = 0;
  pits[8] = 0;
  for (int i = 1; i <= 7; i++) {
    pits[i] = 7;
  }
  for (int i = 9; i <= 15; i++) {
    pits[i] = 7;
  }
  
  firstMove = true;
  p1State = SELECTING;
  p2State = SELECTING;
  p1Selected = 1;
  p2Selected = 9;
  p1CurrentPos = -1;
  p2CurrentPos = -1;
  p1SeedsInHand = 0;
  p2SeedsInHand = 0;
}

void Game::startFirstMove() {
  firstMove = false;
  
  p1SeedsInHand = pits[p1Selected];
  pits[p1Selected] = 0;
  p1CurrentPos = p1Selected;
  p1State = ANIMATING;
  
  p2SeedsInHand = pits[p2Selected];
  pits[p2Selected] = 0;
  p2CurrentPos = p2Selected;
  p2State = ANIMATING;
}

void Game::startP1Move() {
  p1SeedsInHand = pits[p1Selected];
  pits[p1Selected] = 0;
  p1CurrentPos = p1Selected;
  p1State = ANIMATING;
}

void Game::startP2Move() {
  p2SeedsInHand = pits[p2Selected];
  pits[p2Selected] = 0;
  p2CurrentPos = p2Selected;
  p2State = ANIMATING;
}

void Game::animateP1() {
  if (p1SeedsInHand > 0) {
    p1CurrentPos = getNextPos(p1CurrentPos, true);
    pits[p1CurrentPos]++;
    p1SeedsInHand--;
    
    if (p1SeedsInHand == 0) {
      p1State = SELECTING;
      p1Selected = 1;
    }
  }
}

void Game::animateP2() {
  if (p2SeedsInHand > 0) {
    p2CurrentPos = getNextPos(p2CurrentPos, false);
    pits[p2CurrentPos]++;
    p2SeedsInHand--;
    
    if (p2SeedsInHand == 0) {
      p2State = SELECTING;
      p2Selected = 9;
    }
  }
}

int Game::getNextPos(int pos, bool isP1) {
  if (pos >= 1 && pos <= 7) {
    if (pos == 7) return 8;
    return pos + 1;
  }
  else if (pos == 8) {
    return 15;
  }
  else if (pos >= 9 && pos <= 15) {
    if (pos == 9) return 0;
    return pos - 1;
  }
  else if (pos == 0) {
    if (isP1) return 1;
    return 7;
  }
  
  return pos;
}