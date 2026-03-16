#pragma once
#include <string>
#include <vector>

typedef enum {
    SMALL_BLIND,
    BIG_BLIND,
    BOSS_BLIND,
    FINSIHER_BOSS_BLIND,
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

class blind {
    public:
        std::string name;
        std::string description;
        blindType type;
        int reward = 0;
        float thresholdMultiplier = 0;

        blind createSmallBlind();
        blind createBigBlind();
};

class bossBlind : public blind {
    public:
        bossBlindType bossType;
};

bossBlind createBossBlind(int ante, std::vector<bossBlindType> completedBosses);