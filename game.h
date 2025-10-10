#pragma once

#include "deck.h"
#include "window.h"
class game {
public:
    int plays;
    int discards;
    int money;
    int ante;
    int round;
    deck d;
    window levelInfo;
    window gameInfo;
    window mainScreen;
    window specialScreen;
    window cardInfo;


    game();
    void playNextLevel();
};