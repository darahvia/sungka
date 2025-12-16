#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
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
  
  int currentPlayer; // 0 = P1, 1 = P2
  bool gameOver;
  
  String lastRuleMsg; 

  Game();
  void init();
  
  // Move Handling
  void startFirstMove();
  void startP1Move();
  void startP2Move();
  void animateP1();
  void animateP2();
  
  // Helpers
  int getNextPos(int pos, bool isP1);
  int getOppositePit(int pit); 
  void endP1Turn();
  void endP2Turn();
  void checkRoundOneEnd();
  bool hasValidMoves(int player);
  
  // Game Logic
  bool checkGameEnd();
  int getWinner();
  void collectRemainingSeeds();
};

#endif