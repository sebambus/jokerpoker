#pragma once
#include <vector>
#include "deck.h"

class hand : public deck {
public:
    std::vector<bool> selected;
    int cursor;

    hand();
    hand(std::vector<card>);
    void add(card);
    card pop();
    std::vector<card> popSelected();
    void print();
    void moveCursor(int moveBy);
    void sortBySuit();
    void sortByValue();
    int cardsSelected();
private:
    std::vector<card> subsortBySuit(std::vector<card> sub);
    std::vector<card> subsortByValue(std::vector<card> sub);
};