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

    void setupLevel();
    void printLevel();
};