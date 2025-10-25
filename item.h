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

class item {
public:
    int val;
    itemtype type;

    const char* name();
    const char* description();
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