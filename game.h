#pragma once

#include "deck.h"
class game {
public:
    int plays;
    int discards;
    int money;
    int ante;
    int round;
    deck d;

    game();
    void playNextLevel();
};