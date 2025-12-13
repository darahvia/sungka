#ifndef GAME_H
#define GAME_H

#include "Constants.h"

class Game {
public:
  int pits[16];
  bool firstMove;
  
  PlayerState p1State;
  int p1Selected;
  int p1CurrentPos;
  int p1SeedsInHand;
  
  PlayerState p2State;
  int p2Selected;
  int p2CurrentPos;
  int p2SeedsInHand;
  
  Game();
  void init();
  void startFirstMove();
  void startP1Move();
  void startP2Move();
  void animateP1();
  void animateP2();
  int getNextPos(int pos, bool isP1);
};

#endif