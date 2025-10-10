#pragma once
#include <map>
#include <vector>

enum handtype : int;
class hand;
class level;

class scorekeep{
public:
    std::map<handtype, std::vector<int>> scoreTable;
    int currentScore;
    scorekeep();
    int calculateScore(hand played);
};
