#pragma once
#include <string>
#include <vector>

typedef enum {
    SMALL_BLIND,
    BIG_BLIND,
    BLIND_TYPE_COUNT
} blindType;

typedef enum {
    THE_CLUB,
    THE_GOAD,
    THE_HEAD,
    THE_WINDOW,
    THE_HOOK,
    THE_PILLAR,
    THE_PSYCHIC,
    THE_MANACLE,
    THE_ARM,
    THE_FISH,
    THE_FLINT,
    THE_HOUSE,
    THE_MARK,
    THE_MOUTH,
    THE_NEEDLE,
    THE_WALL,
    THE_WATER,
    THE_WHEEL,
    THE_EYE,
    THE_TOOTH,
    THE_PLANT,
    THE_SERPENT,
    THE_OX,
    BOSS_BLIND_TYPE_COUNT
} bossBlindType;

typedef enum {
    UNCOMMON_TAG,
    RARE_TAG,
    FOIL_TAG,
    HOLOGRAPHIC_TAG,
    POLYCHROME_TAG,
    NEGATIVE_TAG,
    INVESTMENT_TAG,
    VOUCHER_TAG,
    BOSS_TAG,
    STANDARD_TAG,
    CHARM_TAG,
    METEOR_TAG,
    BUFFOON_TAG,
    HANDY_TAG,
    GARBAGE_TAG,
    ETHEREAL_TAG,
    COUPON_TAG,
    DOUBLE_TAG,
    JUGGLE_TAG,
    D6_TAG,
    TOP_UP_TAG,
    SPEED_TAG,
    ORBITAL_TAG,
    ECONOMY_TAG,
    TAG_COUNT
} tagType;

class tag {
    public:
        std::string name;
        std::string description;
        tagType type;
        bool immediate = false;
        tag();
        tag(tagType tt);
        void setImmediate();
};

class blind {
    public:
        std::string name;
        std::string description;
        bossBlindType bossType;
        tag skipTag;
        int reward = 0;
        float thresholdMultiplier = 0;
        blind(int ante, std::vector<bossBlindType> completedBosses);
        blind(blindType bt);
        blind();
        
};