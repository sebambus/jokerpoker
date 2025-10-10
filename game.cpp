#include "game.h"

#include "level.h"
#include "window.h"

game::game() :
    levelInfo(10, 20, 0, 0, "Level Info"),
    gameInfo(10, 20, 10, 0, "Game Info"),
    mainScreen(20, 80, 0, 20, ""),
    specialScreen(10, 20, 0, 100, "Jokers and Tarot"),
    cardInfo(10, 20, 10, 100, "Card Info") {
    plays = 3;
    discards = 4;
    money = 0;
    ante = 1;
    round = 1;
    d.fillDeck();
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