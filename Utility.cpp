// Utility.cpp - Implementation of utility functions

#include "Utility.h"

int calculateScore(int reactionTime) {
    if (reactionTime <= 200) return 100;
    else if (reactionTime <= 400) return 90;
    else if (reactionTime <= 600) return 80;
    else if (reactionTime <= 800) return 70;
    else if (reactionTime <= 1000) return 60;
    else if (reactionTime <= 1400) return 50;
    else if (reactionTime <= 2000) return 40;
    else return 30;
}