// MathQuestions.h - Defines math question structure and data

#pragma once
#include <string>

struct MathQuestion {
    std::string question;
    int correctAnswer;
    int wrongAnswer;
};

// Initialize an array of questions and answers
MathQuestion questions[10] = {
    {"2+2=?", 4, 3},
    {"5+3=?", 8, 7},
    {"7-2=?", 5, 6},
    {"3x2=?", 6, 4},
    {"8/4=?", 2, 3},
    {"9-3=?", 6, 8},
    {"7+1=?", 8, 9},
    {"4x2=?", 8, 6},
    {"6/3=?", 2, 1},
    {"5-2=?", 3, 4},
};