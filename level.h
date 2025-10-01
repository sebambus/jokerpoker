#pragma once
#include "scorekeep.h"

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

    void play();
    void setupLevel();
    void printLevel();
    void draw();
    void playHand();
    void discardHand();
};