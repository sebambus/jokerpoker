#pragma once

class game;

#include "deck.h"
#include "hand.h"
#include "window.h"

enum selectableScreen : int{
    CONSUMABLE_SCREEN,
    JOKER_SCREEN
};

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
    
    selectableScreen focusScreen = CONSUMABLE_SCREEN;
    int currConsumable = 0;
    int currJoker = 0;

    level(game*);
    void play();
    void draw();
    void playHand();
    void discardHand();
    void swapFocus();
    void changeConsumable(int by);
    void changeJoker(int by);
    int calculateScore(hand* played);
    void win();
    void lose();
};