#pragma once
#include <string>

enum consumableType : int{
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

class consumable{
public:
    consumableType type;
    const char* name;
    const char* description;
    consumable(consumableType name);
};

const char* consumableName(consumableType type);
const char* consumableDescription(consumableType type);