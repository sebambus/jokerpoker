#include "game.h"

game::game() {
    plays = 3;
    discards = 4;
    money = 0;
    d.fillDeck();
}