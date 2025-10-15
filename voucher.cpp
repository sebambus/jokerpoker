#include "voucher.h"

const char* voucherName(voucher v) {
    switch (v) {
    case OVERSTOCK:
        return "Overstock";
    case CLEARANCE_SALE:
        return "Clearance Sale";
    case HONE:
        return "Hone";
    case REROLL_SURPLUS:
        return "Reroll Surplus";
    case CRYSTAL_BALL:
        return "Crystal Ball";
    case TELESCOPE:
        return "Telescope";
    case GRABBER:
        return "Grabber";
    case WASTEFUL:
        return "Wasteful";
    case TAROT_MERCHANT:
        return "Tarot Merchant";
    case PLANET_MERCHANT:
        return "Planet Merchant";
    case SEED:
        return "Seed";
    case BLANK:
        return "Blank";
    case MAGIC_TRICK:
        return "Magic Trick";
    case HIEROGLYPH:
        return "Hieroglyph";
    case DIRECTORS_CUT:
        return "Directors Cut";
    case PAINT_BRUSH:
        return "Paint Brush";
    case OVERSTOCK_PLUS:
        return "Overstock Plus";
    case LIQUIDATION:
        return "Liquidation";
    case GLOW_UP:
        return "Glow Up";
    case REROLL_GLUT:
        return "Reroll Glut";
    case OMEN_GLOBE:
        return "Omen Globe";
    case NACHO_TONG:
        return "Nacho Tong";
    case RECYCLOMANCY:
        return "Recyclomancy";
    case TAROT_TYCOON:
        return "Tarot Tycoon";
    case PLANET_TYCOON:
        return "Planet Tycoon";
    case MONEY_TREE:
        return "Money Tree";
    case ANTIMATTER:
        return "Antimatter";
    case ILLUSION:
        return "Illusion";
    case PETROGLYPH:
        return "Petroglyph";
    case RETCON:
        return "Retcon";
    case PALETTE:
        return "Palette";
    }
}