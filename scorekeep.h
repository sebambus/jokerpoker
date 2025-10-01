#pragma once
#include <map>
#include "hand.h"



class scorekeep{
public:
    std::map<handtype, std::vector<int>> scoreTable;
    int currentScore;
    scorekeep();
    int calculateScore(hand played);
};
