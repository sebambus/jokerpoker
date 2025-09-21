#include <vector>
#include "card.h"

class deck {
private:
    std::vector<card> cards;
public:
    deck();
    void shuffle();
};