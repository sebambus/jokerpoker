#include "consumable.h"

const char* consumableName(consumable t){
    switch (t) {
        case PLUTO:
            return "Pluto";
        case MERCURY:
            return "Mercury";
        case URANUS:
            return "Uranus";
        case VENUS:
            return "Venus";
        case SATURN:
            return "Saturn";
        case JUPITER:
            return "Jupiter";
        case EARTH:
            return "Earth";
        case MARS:
            return "Mars";
        case NEPTUNE:
            return "Neptune";
        case PLANET_X:
            return "Planet X";
        case CERES:
            return "Ceres";
        case ERIS:
            return "Eris";
    }
    return "ERROR";
}

const char* consumableDescription(consumable t){
    switch (t) {
        case PLUTO:
            return "Increases High Card hand value by +1 Mult and +10 Chips";
        case MERCURY:
            return "Increases Pair hand value by +1 Mult and +15 Chips";
        case URANUS:
            return "Increases Two Pair hand value by +1 Mult and +20 Chips";
        case VENUS:
            return "Increases Three of a Kind hand value by +2 Mult and +20 Chips";
        case SATURN:
            return "Increases Straight hand value by +3 Mult and +30 Chips";
        case JUPITER:
            return "Increases Flush hand value by +2 Mult and +15 Chips";
        case EARTH:
            return "Increases Full House hand value by +2 Mult and +25 Chips";
        case MARS:
            return "Increases Four of a Kind hand value by +3 Mult and +30 Chips";
        case NEPTUNE:
            return "Increases Straight Flush hand value by +4 Mult and +40 Chips";
        case PLANET_X:
            return "Increases Five of a Kind hand value by +3 Mult and +35 Chips";
        case CERES:
            return "Increases Flush House hand value by +4 Mult and +40 Chips";
        case ERIS:
            return "Increases Flush Five hand value by +3 Mult and +50 Chips";
    }
    return "ERROR";
}