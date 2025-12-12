#include "Structures.h"

// convert enum to string for debug in serial
const char* buttonEventToString(ButtonEvent event) {
    switch (event) {
        case P1_LEFT:   return "P1_LEFT";
        case P1_SELECT: return "P1_SELECT";
        case P1_RIGHT:  return "P1_RIGHT";
        case P2_LEFT:   return "P2_LEFT";
        case P2_SELECT: return "P2_SELECT";
        case P2_RIGHT:  return "P2_RIGHT";
        default:        return "NONE";
    }
}