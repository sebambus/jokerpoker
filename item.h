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
    EIGHT_BALL,
    ABSTRACT_JOKER,
    ACROBAT,
    ANCIENT_JOKER,
    ARROWHEAD,
    ASTRONOMER,
    BANNER,
    BARON,
    BASEBALL_CARD,
    BLACKBOARD,
    BLOODSTONE,
    BLUE_JOKER,
    BLUEPRINT,
    BOOTSTRAPS,
    BRAINSTORM,
    BULL,
    BURGLAR,
    BURNT_JOKER,
    BUSINESS_CARD,
    CAMPFIRE,
    CANIO,
    CARD_SHARP,
    CARTOMANCER,
    CASTLE,
    CAVENDISH,
    CEREMONIAL_DAGGER,
    CERTIFICATE,
    CHAOS_THE_CLOWN,
    CHICOT,
    CLEVER_JOKER,
    CLOUD_NINE,
    CONSTELLATION,
    CRAFTY_JOKER,
    CRAZY_JOKER,
    CREDIT_CARD,
    DELAYED_GRATIFICATION,
    DEVIOUS_JOKER,
    DIET_COLA,
    DNA,
    DRIVERS_LICENSE,
    DROLL_JOKER,
    DRUNKARD,
    DUSK,
    EGG,
    EROSION,
    EVEN_STEVEN,
    FACELESS_JOKER,
    FIBONACCI,
    FLASH_CARD,
    FLOWER_POT,
    FORTUNE_TELLER,
    FOUR_FINGERS,
    GIFT_CARD,
    GLASS_JOKER,
    GLUTTONOUS_JOKER,
    GOLDEN_JOKER,
    GOLDEN_TICKET,
    GREEDY_JOKER,
    GREEN_JOKER,
    GROS_MICHEL,
    HACK,
    HALF_JOKER,
    HALLUCINATION,
    HANGING_CHAD,
    HIKER,
    HIT_THE_ROAD,
    HOLOGRAM,
    ICE_CREAM,
    INVISIBLE_JOKER,
    BASIC_JOKER,
    JOKER_STENCIL,
    JOLLY_JOKER,
    JUGGLER,
    LOYALTY_CARD,
    LUCHADOR,
    LUCKY_CAT,
    LUSTY_JOKER,
    MAD_JOKER,
    MADNESS,
    MAIL_IN_REBATE,
    MARBLE_JOKER,
    MATADOR,
    MERRY_ANDY,
    MIDAS_MASK,
    MIME,
    MISPRINT,
    MR_BONES,
    MYSTIC_SUMMIT,
    OBELISK,
    ODD_TODD,
    ONYX_AGATE,
    OOPS_ALL_6S,
    PAREIDOLIA,
    PERKEO,
    PHOTOGRAPH,
    POPCORN,
    RAISED_FIST,
    RAMEN,
    RED_CARD,
    RESERVED_PARKING,
    RIDE_THE_BUS,
    RIFF_RAFF,
    ROCKET,
    ROUGH_GEM,
    RUNNER,
    SATELLITE,
    SCARY_FACE,
    SCHOLAR,
    SEANCE,
    SEEING_DOUBLE,
    SELTZER,
    SHOOT_THE_MOON,
    SHORTCUT,
    SHOWMAN,
    SIXTH_SENSE,
    SLY_JOKER,
    SMEARED_JOKER,
    SMILEY_FACE,
    SOCK_AND_BUSKIN,
    SPACE_JOKER,
    SPARE_TROUSERS,
    SPLASH,
    SQUARE_JOKER,
    STEEL_JOKER,
    STONE_JOKER,
    STUNTMAN,
    SUPERNOVA,
    SUPERPOSITION,
    SWASHBUCKLER,
    THE_DUO,
    THE_FAMILY,
    THE_IDOL,
    THE_ORDER,
    THE_TRIBE,
    THE_TRIO,
    THROWBACK,
    TO_DO_LIST,
    TO_THE_MOON,
    TRADING_CARD,
    TRIBOULET,
    TROUBADOUR,
    TURTLE_BEAN,
    VAGABOND,
    VAMPIRE,
    WALKIE_TALKIE,
    WEE_JOKER,
    WILY_JOKER,
    WRATHFUL_JOKER,
    YORICK,
    ZANY_JOKER,
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