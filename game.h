#pragma once
#include "deck.h"

class game {
public:
    int plays;
    int discards;
    int money;
    deck d;
    int ante;
    int round;

    game();
    void playNextLevel();
};