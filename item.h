#pragma once
#include <string>

enum itemtype : int {
    PLANET,
    TAROT,
    SPECTRAL,
    JOKER,
    CARD,
    VOUCHER
};

struct item {
    int val;
    itemtype type;
};

enum planet : int {
    PLUTO,
    MERCURY,
    URANUS,
    VENUS,
    SATURN,
    JUPITER,
    EARTH,
    MARS,
    NEPTUNE,
    PLANET_X,
    CERES,
    ERIS
};

const char* itemName(item type);
const char* itemDescription(item type);