#pragma once
#include <vector>
#include "deck.h"

class hand : public deck {
public:
    std::vector<bool> selected;

    void add(card);
    card pop();
    std::vector<card> popSelected();
    void print();
};