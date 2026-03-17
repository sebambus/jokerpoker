#include "blind.h"
#include "readcsv.h"
#include <algorithm>
#include "debug.h"

blind::blind(){

}

// for small or big blinds
blind::blind(blindType bt){
    if (bt == SMALL_BLIND){
        name = "Small Blind";
        reward = 3;
        thresholdMultiplier = 1;
    } else {
        name = "Big Blind";
        reward = 4;
        thresholdMultiplier = 1.5;
    }
    description = "";
    bossType = BOSS_BLIND_TYPE_COUNT;
    skipTag = tag();
}

// for boss blinds
blind::blind(int ante, std::vector<bossBlindType> completedBosses){
    // currently does not work with endless mode, can change later
    // create vector of boss blinds to choose from given those already completed and the ante
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

    bossType = (bossBlindType)(rand() % possibleTypes.size());
    name = std::string(readcsv("blind.csv", (int)bossType + 1, 0));
    description = std::string(readcsv("blind.csv", (int)bossType + 1, 1));
    thresholdMultiplier = atoi(readcsv("blind.csv", (int)bossType + 1, 3));
    reward = atoi(readcsv("blind.csv", (int)bossType + 1, 4));
    skipTag = tag(TAG_COUNT);
}

tag::tag(){
    type = (tagType)(rand() % TAG_COUNT);
    name = std::string(readcsv("tag.csv", (int)type + 1, 0));
    description = std::string(readcsv("tag.csv", (int)type + 1, 1));
    setImmediate();
}

tag::tag(tagType tt){
    type = tt;
    if (type = TAG_COUNT) return;
    name = std::string(readcsv("tag.csv", (int)type + 1, 0));
    description = std::string(readcsv("tag.csv", (int)type + 1, 1));
    setImmediate();
}

void tag::setImmediate(){
    switch(type){
        case STANDARD_TAG:
        case CHARM_TAG:
        case METEOR_TAG:
        case BUFFOON_TAG:
        case HANDY_TAG:
        case GARBAGE_TAG:
        case ETHEREAL_TAG:
        case TOP_UP_TAG:
        case SPEED_TAG:
        case ORBITAL_TAG:
        case ECONOMY_TAG:
            immediate = true;
            break;
        default:
            immediate = false;
    }
}