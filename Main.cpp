// Main.cpp - Main function for the program

#include "HardwareConfig.h"
#include "GameLogic.h"


using namespace mbed;

int main() {
    srand((unsigned) time(NULL)); 
    lcd.cls();
    lcd.printf("Select game mode");

    int gameMode = selectGameMode(lcd);
    int rounds = getNumberOfRounds(lcd);

    if (quitGame) {
        lcd.cls();
        lcd.printf("Quitting game.");
        return 0;
    }

    if (gameMode == 1) {
        startGame(rounds, lcd);
    } else if (gameMode == 2) {
        startLearningGame(rounds, lcd);
    }

    return 0;
}