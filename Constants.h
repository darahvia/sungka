#ifndef CONSTANTS_H
#define CONSTANTS_H

// Button pins on PCF8575
#define BTN_P1_LEFT   12
#define BTN_P1_SELECT 13
#define BTN_P1_RIGHT  3
#define BTN_P2_LEFT   14
#define BTN_P2_SELECT 7
#define BTN_P2_RIGHT  15

// Animation timing
#define ANIM_DELAY 300
#define BUTTON_DELAY 50
#define AI_MOVE_DELAY 1000
#define AI_THINK_DELAY 500

// Screen states
enum Screen {
  MENU,
  PVP,
  PVA,
  LEADERBOARD
};

// Player states
enum PlayerState {
  SELECTING,
  READY,
  ANIMATING,
  WAITING
};

#endif