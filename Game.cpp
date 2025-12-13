#include "Game.h"

Game::Game() {
  init();
}

void Game::init() {
  // Player 1: houses [1-7], STORE [0] (Left)
  pits[0] = 0;
  for (int i = 1; i <= 7; i++) {
    pits[i] = 7;
  }
  
  // Player 2: houses [8-14], STORE [15] (Right)
  for (int i = 8; i <= 14; i++) {
    pits[i] = 7;
  }
  pits[15] = 0;
  
  firstMove = true;
  p1State = SELECTING;
  p2State = SELECTING;
  
  // Default selections
  p1Selected = 4;
  p2Selected = 11;
  
  p1CurrentPos = -1;
  p2CurrentPos = -1;
  p1SeedsInHand = 0;
  p2SeedsInHand = 0;
  
  currentPlayer = 0; 
  gameOver = false;
  lastRuleMsg = "Game Start!";
}

// Helper for your specific board mapping:
// 1-8, 2-9, 3-10, 4-11, 5-12, 6-13, 7-14
int Game::getOppositePit(int pit) {
  if (pit >= 1 && pit <= 7) {
    return pit + 7; // e.g., 1->8, 7->14
  } else if (pit >= 8 && pit <= 14) {
    return pit - 7; // e.g., 8->1, 14->7
  }
  return -1;
}

void Game::startFirstMove() {
  lastRuleMsg = "Round 1: Simultaneous";
  if (pits[p1Selected] > 0) {
    p1SeedsInHand = pits[p1Selected];
    pits[p1Selected] = 0;
    p1CurrentPos = p1Selected;
    p1State = ANIMATING;
  }
  
  if (pits[p2Selected] > 0) {
    p2SeedsInHand = pits[p2Selected];
    pits[p2Selected] = 0;
    p2CurrentPos = p2Selected;
    p2State = ANIMATING;
  }
}

void Game::startP1Move() {
  if (pits[p1Selected] == 0) return;
  
  p1SeedsInHand = pits[p1Selected];
  pits[p1Selected] = 0;
  p1CurrentPos = p1Selected;
  p1State = ANIMATING;
  lastRuleMsg = ""; 
}

void Game::startP2Move() {
  if (pits[p2Selected] == 0) return;
  
  p2SeedsInHand = pits[p2Selected];
  pits[p2Selected] = 0;
  p2CurrentPos = p2Selected;
  p2State = ANIMATING;
  lastRuleMsg = "";
}

void Game::animateP1() {
  if (p1SeedsInHand > 0) {
    p1CurrentPos = getNextPos(p1CurrentPos, true);
    
    // Drop one stone
    pits[p1CurrentPos]++;
    p1SeedsInHand--;
    
    // Check if this was the last stone
    if (p1SeedsInHand == 0) {
      
      // CASE 1: Landed in Own Store (Extra Turn)
      if (p1CurrentPos == 0) {
        lastRuleMsg = "P1 HOME BASE: Extra Turn!";
        Serial.println("P1 Landed in Store - Extra Turn");
        
        if (checkGameEnd()) return;

        if (firstMove) {
          p1State = WAITING; 
          checkRoundOneEnd();
        } else {
          if (hasValidMoves(0)) {
            p1State = SELECTING;
            p1Selected = 4;
          } else {
             endP1Turn();
          }
        }
        return;
      }
      
      // CASE 2: Landed in a Pit (1-14)
      if (pits[p1CurrentPos] > 1) {
        // Chain rule: Pick up everything and continue
        p1SeedsInHand = pits[p1CurrentPos];
        pits[p1CurrentPos] = 0;
        return; 
      }
      
      // CASE 3: Landed in Empty Pit (seeds == 1 after drop)
      else if (pits[p1CurrentPos] == 1) {
        // If on OWN side -> Check Capture
        if (p1CurrentPos >= 1 && p1CurrentPos <= 7) {
           int oppositePos = getOppositePit(p1CurrentPos);
           int oppSeeds = pits[oppositePos];
           
           if (oppSeeds > 0) {
             // CAPTURE RULE: Take own seed + opponent seeds
             int captured = oppSeeds + 1; 
             
             // Update Data
             pits[oppositePos] = 0;
             pits[p1CurrentPos] = 0;
             pits[0] += captured; // Put in P1 Store
             
             // Message: "P1 Captured [OpponentPit](Seeds)"
             lastRuleMsg = "P1 CAPTURE! Pit " + String(p1CurrentPos) + " took Pit " + String(oppositePos) + "(" + String(oppSeeds) + ")";
             Serial.print("P1 Captured "); Serial.println(captured);
           } else {
             lastRuleMsg = "P1: Landed on empty pit " + String(p1CurrentPos);
           }
        } else {
           lastRuleMsg = "P1: End Turn (Opponent Side)";
        }
        
        // Turn Ends
        if (firstMove) {
          p1State = WAITING;
          checkRoundOneEnd();
        } else {
          endP1Turn();
        }
      }
    }
  }
}

void Game::animateP2() {
  if (p2SeedsInHand > 0) {
    p2CurrentPos = getNextPos(p2CurrentPos, false);
    
    // Drop one stone
    pits[p2CurrentPos]++;
    p2SeedsInHand--;
    
    // Check if this was the last stone
    if (p2SeedsInHand == 0) {
      
      // CASE 1: Landed in Own Store (Extra Turn)
      if (p2CurrentPos == 15) {
        lastRuleMsg = "P2 HOME BASE: Extra Turn!";
        Serial.println("P2 Landed in Store - Extra Turn");
        
        if (checkGameEnd()) return;

        if (firstMove) {
          p2State = WAITING;
          checkRoundOneEnd();
        } else {
          if (hasValidMoves(1)) {
            p2State = SELECTING;
            p2Selected = 11;
          } else {
            endP2Turn();
          }
        }
        return;
      }
      
      // CASE 2: Landed in a Pit (1-14)
      if (pits[p2CurrentPos] > 1) {
        p2SeedsInHand = pits[p2CurrentPos];
        pits[p2CurrentPos] = 0;
        return; 
      }
      
      // CASE 3: Landed in Empty Pit (seeds == 1)
      else if (pits[p2CurrentPos] == 1) {
        // If on OWN side -> Check Capture
        if (p2CurrentPos >= 8 && p2CurrentPos <= 14) {
           int oppositePos = getOppositePit(p2CurrentPos);
           int oppSeeds = pits[oppositePos];
           
           if (oppSeeds > 0) {
             // CAPTURE RULE
             int captured = oppSeeds + 1;
             
             pits[oppositePos] = 0;
             pits[p2CurrentPos] = 0;
             pits[15] += captured; // Put in P2 Store
             
             lastRuleMsg = "P2 CAPTURE! Pit " + String(p2CurrentPos) + " took Pit " + String(oppositePos) + "(" + String(oppSeeds) + ")";
             Serial.print("P2 Captured "); Serial.println(captured);
           } else {
             lastRuleMsg = "P2: Landed on empty pit " + String(p2CurrentPos);
           }
        } else {
           lastRuleMsg = "P2: End Turn (Opponent Side)";
        }
        
        // Turn Ends
        if (firstMove) {
          p2State = WAITING;
          checkRoundOneEnd();
        } else {
          endP2Turn();
        }
      }
    }
  }
}

void Game::endP1Turn() {
  if (checkGameEnd()) return;
  
  currentPlayer = 1; // Set to P2
  p1State = WAITING;
  
  if (hasValidMoves(1)) {
    p2State = SELECTING;
    p2Selected = 11;
  } else {
    lastRuleMsg = "P2 No Moves! P1 Goes Again.";
    Serial.println("P2 has no moves! Skipping.");
    currentPlayer = 0;
    p2State = WAITING;
    p1State = SELECTING;
    
    if (!hasValidMoves(0)) {
      gameOver = true;
      collectRemainingSeeds();
    }
  }
}

void Game::endP2Turn() {
  if (checkGameEnd()) return;
  
  currentPlayer = 0; // Set to P1
  p2State = WAITING;
  
  if (hasValidMoves(0)) {
    p1State = SELECTING;
    p1Selected = 4;
  } else {
    lastRuleMsg = "P1 No Moves! P2 Goes Again.";
    Serial.println("P1 has no moves! Skipping.");
    currentPlayer = 1;
    p1State = WAITING;
    p2State = SELECTING;
    
    if (!hasValidMoves(1)) {
      gameOver = true;
      collectRemainingSeeds();
    }
  }
}

void Game::checkRoundOneEnd() {
  if (p1State == WAITING && p2State == WAITING) {
    lastRuleMsg = "R1 Done. P1 Starts R2.";
    Serial.println("Round 1 Finished. Starting Turn-based Round 2.");
    firstMove = false;
    
    currentPlayer = 0;
    p1State = SELECTING;
    p2State = WAITING;
    p1Selected = 4;
    p2Selected = 11;
  }
}

int Game::getNextPos(int pos, bool isP1) {
  int next = pos;
  
  if (pos >= 1 && pos < 7) next = pos + 1;
  else if (pos == 7) next = 15;
  else if (pos == 15) next = 14;
  else if (pos > 8 && pos <= 14) next = pos - 1;
  else if (pos == 8) next = 0;
  else if (pos == 0) next = 1;
  
  if (isP1 && next == 15) return 14;
  if (!isP1 && next == 0) return 1;
  
  return next;
}

bool Game::hasValidMoves(int player) {
  int start = (player == 0) ? 1 : 8;
  int end = (player == 0) ? 7 : 14;
  
  for (int i = start; i <= end; i++) {
    if (pits[i] > 0) return true;
  }
  return false;
}

bool Game::checkGameEnd() {
  if (gameOver) return true;
  return false;
}

void Game::collectRemainingSeeds() {
  for (int i = 1; i <= 7; i++) {
    pits[0] += pits[i];
    pits[i] = 0;
  }
  
  for (int i = 8; i <= 14; i++) {
    pits[15] += pits[i];
    pits[i] = 0;
  }
  lastRuleMsg = "GAME OVER!";
}

int Game::getWinner() {
  if (pits[0] > pits[15]) return 1;
  if (pits[15] > pits[0]) return 2;
  return 0;
}