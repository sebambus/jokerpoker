#pragma once

class game;
class window;

#include "scorekeep.h"
#include "deck.h"
#include "hand.h"
class level {
private:
    game* g;
    int discards;
    int plays;
    scorekeep tally;
    int recentScore;
    int threshold;
    deck d;
    hand h;
    hand played;

    void play();
    void draw();
    void playHand();
    void discardHand();
    void updateInfo(window);
    void updateScreen(window);

public:
    level(game*);
};