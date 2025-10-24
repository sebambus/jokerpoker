#pragma once
#include <string>

enum consumabletype : int {
    PLANET,
    TAROT,
    SPECTRAL,
    JOKER,
    CARD,
    VOUCHER
};

struct consumable {
    int val;
    consumabletype type;
};

enum planet : int{
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

const char* consumableName(consumable type);
const char* consumableDescription(consumable type);