// HardwareConfig.h - Hardware configuration for the game

#include "mbed.h"
#include "buzzer.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Keypad.h"
#include "LCDi2c.h" 
#include <vector>

LCDi2c lcd(PTC11, PTC10, LCD20x4);
Keypad kpad(PTA6, PTA4, PTA26, PTA27, PTA1, PTA9, PTA8, PTA7);
DigitalOut extLed(PTC6); 
Beep buzzer(PTD3);
std::vector<int> roundScores;
InterruptIn irSensor1(PTC2, PullDown);
InterruptIn irSensor2(PTC3, PullDown); 
InterruptIn irSensor3(PTC4, PullDown); 
DigitalIn buttonLED(PTC5, PullUp);
DigitalIn buttonBuzzer(PTC12, PullUp);
DigitalOut led(LED1);

Timer reactionTimer;
volatile bool falseStart = false;
bool quitGame = false;