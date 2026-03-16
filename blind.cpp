#include "blind.h"
#include "readcsv.h"
#include <algorithm>
#include "debug.h"

blind blind::createSmallBlind(){
    blind b;
    b.name = "Small Blind";
    b.description = "";
    b.reward = 3;
    b.thresholdMultiplier = 1;
    b.type = SMALL_BLIND;
    return b;
}

blind blind::createBigBlind(){
    blind b;
    b.name = "Big Blind";
    b.description = "";
    b.reward = 4;
    b.thresholdMultiplier = 1.5;
    b.type = BIG_BLIND;
    return b;
}

bossBlind createBossBlind(int ante, std::vector<bossBlindType> completedBosses){
    bossBlind b;
    b.type = BOSS_BLIND;

    std::vector<bossBlindType> possibleTypes;
    for (int i = 0; i < (int)BOSS_BLIND_TYPE_COUNT; i++)
    {
        bool isCompleted = false;
        bool inAnteRange = false;
        // check if blind has already been completed
        for (bossBlindType bbt : completedBosses){
            if (bbt == (bossBlindType)i) isCompleted = true;
        }
        // check ante requirement for that blind
        int anteReq = atoi(readcsv("blind.csv", i + 1, 2));
        if (ante >= anteReq) inAnteRange = true;
        if (!isCompleted && inAnteRange) possibleTypes.push_back((bossBlindType)i);
    }

    b.bossType = (bossBlindType)(rand() % possibleTypes.size());
    b.name = std::string(readcsv("blind.csv", (int)b.bossType + 1, 0));
    b.description = std::string(readcsv("blind.csv", (int)b.bossType + 1, 1));
    b.thresholdMultiplier = atoi(readcsv("blind.csv", (int)b.bossType + 1, 3));
    b.reward = atoi(readcsv("blind.csv", (int)b.bossType + 1, 4));

    return b;
    //check for if there are no possible blinds
}