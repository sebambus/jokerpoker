#pragma once
#include <string>

enum consumable : int{
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