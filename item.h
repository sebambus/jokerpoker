#pragma once
#include <string>

class card;

enum itemtype : int {
    PLANET,
    TAROT,
    SPECTRAL,
    JOKER,
    VOUCHER,
    CARD
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
    ERIS,
    PLANET_COUNT
};

enum tarot : int {
    TAROT_ONE,
    TAROT_TWO,
    TAROT_COUNT
};

enum spectral : int {
    SPECTRAL_ONE,
    SPECTRAL_TWO,
    SPECTRAL_COUNT
};

enum joker : int {
    JOKER_ONE,
    JOKER_TWO,
    JOKER_COUNT
};

enum voucher : int {
    OVERSTOCK,      // needs to be implemented
    CLEARANCE_SALE, // needs to be implemented
    HONE,           // needs to be implemented
    REROLL_SURPLUS, // needs to be implemented
    CRYSTAL_BALL,   // needs to be implemented
    TELESCOPE,      // needs to be implemented
    GRABBER,
    WASTEFUL,
    TAROT_MERCHANT, // needs to be implemented
    PLANET_MERCHANT,// needs to be implemented
    SEED,           // needs to be implemented
    BLANK,          // needs to be implemented
    MAGIC_TRICK,    // needs to be implemented
    HIEROGLYPH,     // needs to be implemented
    DIRECTORS_CUT,  // needs to be implemented
    PAINT_BRUSH,    // needs to be implemented
    OVERSTOCK_PLUS, // needs to be implemented
    LIQUIDATION,    // needs to be implemented
    GLOW_UP,        // needs to be implemented
    REROLL_GLUT,    // needs to be implemented
    OMEN_GLOBE,     // needs to be implemented
    OBSERVATORY,    // needs to be implemented
    NACHO_TONG,
    RECYCLOMANCY,
    TAROT_TYCOON,   // needs to be implemented
    PLANET_TYCOON,  // needs to be implemented
    MONEY_TREE,     // needs to be implemented
    ANTIMATTER,     // needs to be implemented
    ILLUSION,       // needs to be implemented
    PETROGLYPH,     // needs to be implemented
    RETCON,         // needs to be implemented
    PALETTE,        // needs to be implemented
    VOUCHER_COUNT
};

class item {
public:
    int val;
    itemtype type;

    item(planet);
    item(tarot);
    item(spectral);
    item(joker);
    item(voucher);
    item(card);
    const char* name();
    const char* description();
};