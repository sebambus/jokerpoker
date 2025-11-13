#pragma once

class game;

#include "deck.h"
#include "hand.h"
#include "window.h"


class level {
public:
    game* g;
    int discards;
    int plays;
    int currentScore;
    int recentScore;
    int threshold;
    deck d;
    hand h;
    hand played;

    int recentChips = 0;
    int recentMult = 0;

    level(game*);
    void play();
    void draw();
    void playHand();
    void discardHand();
    int calculateScore(hand* played, hand * h);
    void win();
    void lose();
};