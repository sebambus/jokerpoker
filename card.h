using namespace std;
#include <string>

enum suit {
    SPADE,
    CLUB,
    HEART,
    DIAMOND
};

class card{
    int cardValue;
    suit cardSuit;

    card(int value, suit suit);
};