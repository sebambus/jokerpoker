#pragma once
#include "scorekeep.h"
#include "game.h"

class level {
public:
    game* g;
    int discards;
    int plays;
    scorekeep tally;
    int recentScore;
    int threshold;
    deck d;
    hand h;
    hand played;

    level(game*, int);
    void play();
    void draw();
    void playHand();
    void discardHand();
};