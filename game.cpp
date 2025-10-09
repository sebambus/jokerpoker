#include "game.h"

#include "level.h"

game::game() {
    plays = 3;
    discards = 4;
    money = 0;
    d.fillDeck();
    ante = 1;
    round = 1;
}

void game::playNextLevel() {
    int antebases[9] = {100, 300, 800, 2000, 5000, 11000, 20000, 35000, 50000};
    int threshold = antebases[ante] * (round+1)/2;

    level l(this, threshold);
    l.play();

    round++;
    if(round > 3) {
        round = 1;
        ante++;
    }
}