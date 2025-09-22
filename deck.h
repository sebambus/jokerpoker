#pragma once

#include <vector>
#include "card.h"

class deck {
public:
    std::vector<card> cards;

    deck();
    void shuffle();
};