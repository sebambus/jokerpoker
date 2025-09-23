#pragma once

enum suit {
    SPADE,
    CLUB,
    HEART,
    DIAMOND
};

class card{
public:
    int cardValue = 0;
    suit cardSuit = SPADE;

    card(int v, suit s);
    void print();
};