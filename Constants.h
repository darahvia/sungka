#ifndef CONSTANTS_H
#define CONSTANTS_H

// Button pins on PCF8575
#define BTN_P1_LEFT   12  // 14
#define BTN_P1_SELECT 13  // 15
#define BTN_P1_RIGHT  27
#define BTN_P2_LEFT   15  // 17
#define BTN_P2_SELECT 7  // 7
#define BTN_P2_RIGHT  14  // 16

// Animation timing
#define ANIM_DELAY 250
#define BUTTON_DELAY 50
#define AI_MOVE_DELAY 1000
#define AI_THINK_DELAY 500

// Screen states
enum Screen {
  MENU,
  PVP,
  PVA,
  RULES
};

// Player states
enum PlayerState {
  SELECTING,
  READY,
  ANIMATING,
  WAITING
};

#endif