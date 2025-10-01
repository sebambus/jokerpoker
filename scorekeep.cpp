#include "scorekeep.h"
#include "hand.h"
#include <iostream>
#include <string>

scorekeep::scorekeep(){
    currentScore = 0;

    scoreTable[FIVE_FLUSH]     = {160, 16};
    scoreTable[HOUSE_FLUSH]    = {140, 14};
    scoreTable[FIVE_KIND]      = {120, 12};
    scoreTable[ROYAL_FLUSH]    = {100, 8};
    scoreTable[STRAIGHT_FLUSH] = {100, 8};
    scoreTable[FOUR_KIND]      = {60, 7};
    scoreTable[FULL_HOUSE]     = {40, 4};
    scoreTable[FLUSH]          = {35, 4};
    scoreTable[STRAIGHT]       = {30, 4};
    scoreTable[THREE_KIND]     = {30, 3};
    scoreTable[TWO_PAIR]       = {20, 2};
    scoreTable[PAIR]           = {10, 2};
    scoreTable[HIGH]           = {5, 1};
}

int scorekeep::calculateScore(hand played){
    std::pair<handtype, std::vector<card>> scoringInfo;
    scoringInfo = played.scoreTypeAndCards();
    handtype type = scoringInfo.first;
    std::vector<card> scoredCards = scoringInfo.second;
    int flat = scoreTable[type][0];
    int mult = scoreTable[type][1];
    int score = flat;

    for (int i = 0; i < scoredCards.size(); i++)
    {
        score += scoredCards[i].cardValue;
    }

    score *= mult;
    currentScore += score;

    return score;
}
