#pragma once
#include "scorekeep.h"
#include "game.h"

class level {
public:
    int discards;
    int plays;
    scorekeep tally;
    int recentScore;
    int threshold;
    deck d;
    hand h;
    hand played;

    level(game, int);
    void play();
    void setupLevel();
    void printLevel();
    void draw();
    void playHand();
    void discardHand();
};