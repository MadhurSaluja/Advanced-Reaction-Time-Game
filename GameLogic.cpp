// GameLogic.cpp - Implementation of game logic functions

#include "GameLogic.h"
#include "HardwareConfig.h"

int selectGameMode(LCDi2c &lcd) {
    lcd.cls();
    lcd.printf("1.Fun 2.Learn");
    while (true) {
        if (buttonLED.read() == 0) {  
            return 1;  // Fun Game
        } else if (buttonBuzzer.read() == 0) {  
            return 2;  // Learning Game
        }
        ThisThread::sleep_for(100ms);
    }
}

int getNumberOfRounds(LCDi2c &lcd) {
    std::string input = "";
    char key;
    int rounds = 0;

    lcd.cls();
    lcd.printf("Rounds (2-10):#");
    while (true) {
        key = kpad.ReadKey();
        if (key == '*') {
            quitGame = true;
            return 0;
        }
        if (key != '\0') {
            if (key == '#') {
                rounds = std::stoi(input);
                if (rounds >= 2 && rounds <= 10) {
                    break;
                } else {
                    lcd.cls();
                    lcd.printf("Enter 2-10:#");
                    input = "";
                }
            } else {
                input += key;
                lcd.putc(key);
            }
        }
        ThisThread::sleep_for(100ms);
    }
    return rounds;
}


// Ensure this is called once at the start of the main() function


void startGame(int rounds, LCDi2c &lcd) {
    int totalScore = 0;
    roundScores.clear();

    for (int i = 1; i <= rounds; i++) {
        falseStart = false;
        lcd.cls();
        lcd.printf("Round %d", i);

        // Set up interrupt handlers for the IR sensors
        irSensor1.rise(&obstacleDetected);
        irSensor2.rise(&obstacleDetected);
        irSensor3.rise(&obstacleDetected);

        // Random delay loop
        for (int j = 0; j < (rand() % 6 + 10); j++) {
            ThisThread::sleep_for(1s);
            if (falseStart) {
                lcd.cls();
                lcd.printf("False start!\nScore: 0");
                roundScores.push_back(0);
                break;
            }
        }

        // Disconnect the interrupt handlers
        irSensor1.rise(nullptr);
        irSensor2.rise(nullptr);
        irSensor3.rise(nullptr);

        if (falseStart) {
            ThisThread::sleep_for(8s);
            continue;
        }

        // Randomly select the event for LED or buzzer
        bool eventSelected = rand() % 2;  // Randomly select between 0 or 1

        reactionTimer.reset();
        reactionTimer.start();

        // Activate either the LED or the buzzer based on the random selection
        if (eventSelected) {
            extLed = 1;  // Turn on external LED
            lcd.printf("\nLED on");
        } else {
            buzzer.beep(1000, 3);
            lcd.printf("\nBuzzer on");
        }

        // Check for button press and calculate score
        while (true) {
            if ((buttonLED.read() == 0 && eventSelected) || (buttonBuzzer.read() == 0 && !eventSelected)) {
                reactionTimer.stop();
                int reactionTime = reactionTimer.read_ms();
                int roundScore = calculateScore(reactionTime);
                totalScore += roundScore;
                if (eventSelected) extLed = 0;  // Turn off external LED
                else buzzer.nobeep();
                lcd.cls();
                lcd.printf("Reaction time:\n%d ms\nScore: %d", reactionTime, roundScore);
                roundScores.push_back(roundScore);
                break;
            }
            ThisThread::sleep_for(10ms);
        }

        ThisThread::sleep_for(8s);
    }

    // Output the total score on the LCD
    lcd.cls();
    lcd.printf("Total score: %d", totalScore);

    // Output all round scores to the terminal
    std::cout << std::endl; // Make sure to end the previous line
    for (const int& score : roundScores) {
        std::cout << score << std::endl; // Output each score on a new line
    }
    std::cout << std::endl;
}

void startLearningGame(int rounds, LCDi2c &lcd) {
    int totalScore = 0;
    roundScores.clear();  // Ensure the vector is empty before starting

    for (int i = 1; i <= rounds; i++) {
        lcd.cls();
        lcd.printf("Learning Round %d", i);
        falseStart = false;
        irSensor1.rise(&obstacleDetected);
        irSensor2.rise(&obstacleDetected);
        irSensor3.rise(&obstacleDetected);

        ThisThread::sleep_for(8s);

        irSensor1.rise(nullptr);
        irSensor2.rise(nullptr);
        irSensor3.rise(nullptr);
        if (falseStart) {
            lcd.cls();
            lcd.printf("False start!\nScore: 0");
            roundScores.push_back(0);  // Store the score for this round
            ThisThread::sleep_for(2s);
            continue;
        }

        MathQuestion &selectedQuestion = questions[rand() % 10];
        int correctButton = rand() % 2;
        lcd.cls();
        if (correctButton == 0) {
            lcd.printf("%s\n1.%d  2.%d", selectedQuestion.question.c_str(), selectedQuestion.correctAnswer, selectedQuestion.wrongAnswer);
        } else {
            lcd.printf("%s\n1.%d  2.%d", selectedQuestion.question.c_str(), selectedQuestion.wrongAnswer, selectedQuestion.correctAnswer);
        }

        reactionTimer.reset();
        reactionTimer.start();
        bool gotAnswer = false;

        while (!gotAnswer) {
            if (buttonLED.read() == 0) {
                reactionTimer.stop();
                int reactionTime = reactionTimer.read_ms();
                if (correctButton == 0) {
                    int roundScore = calculateScore(reactionTime);
                    totalScore += roundScore;
                    lcd.cls();
                    lcd.printf("Correct!\nTime: %d ms\nScore: %d", reactionTime, roundScore);
                    roundScores.push_back(roundScore);  // Store the score for this round
                } else {
                    lcd.cls();
                    lcd.printf("Wrong!\nTime: %d ms\nScore: 0", reactionTime);
                    roundScores.push_back(0);  // Store the score for this round
                }
                gotAnswer = true;
            } else if (buttonBuzzer.read() == 0) {
                reactionTimer.stop();
                int reactionTime = reactionTimer.read_ms();
                if (correctButton == 1) {
                    int roundScore = calculateScore(reactionTime);
                    totalScore += roundScore;
                    lcd.cls();
                    lcd.printf("Correct!\nTime: %d ms\nScore: %d", reactionTime, roundScore);
                    roundScores.push_back(roundScore);  // Store the score for this round
                } else {
                    lcd.cls();
                    lcd.printf("Wrong!\nTime: %d ms\nScore: 0", reactionTime);
                    roundScores.push_back(0);  // Store the score for this round
                }
                gotAnswer = true;
            }
            ThisThread::sleep_for(100ms);
        }

        ThisThread::sleep_for(8s);
    }

    // Output all round scores on a single line
    std::cout << std::endl; // Make sure to end the previous line
    for (const int& score : roundScores) {
        std::cout << score << std::endl; // Output each score on a new line
    }
    std::cout << std::endl;
    // Then print the total score on the LCD
    lcd.cls();
    lcd.printf("Total score: %d", totalScore);
}


void obstacleDetected() {
    if (irSensor1.read() == 1 || irSensor2.read() == 1 || irSensor3.read() == 1) {
        falseStart = true;
    }
}
