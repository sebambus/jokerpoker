#pragma once

class game;

#include "scorekeep.h"
#include "deck.h"
#include "hand.h"
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