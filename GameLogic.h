// GameLogic.h - Header file for game logic functions

#pragma once
#include "LCDi2c.h"
#include "MathQuestions.h"

int selectGameMode(LCDi2c &lcd);
int getNumberOfRounds(LCDi2c &lcd);
void startGame(int rounds, LCDi2c &lcd);
void startLearningGame(int rounds, LCDi2c &lcd);
void obstacleDetected();
