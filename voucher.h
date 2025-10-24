#pragma once

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

const char* voucherName(voucher);