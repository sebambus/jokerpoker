#include "game.h"

#include "level.h"
#include "window.h"
#include "item.h"
#include "readcsv.h"

game::game() :
    levelInfo(10, 20, 0, 0, "Level Info"),
    gameInfo(10, 20, 10, 0, "Game Info"),
    mainScreen(20, 80, 0, 20, ""),
    specialScreen(10, 20, 0, 100, "Consumables"),
    cardInfo(10, 20, 10, 100, "Card Info"),
    peekScreen(10, 100, 20, 20, "Peek") {
    money = 0;
    ante = 1;
    round = 1;
    d.fillDeck();
    initHandTable();

    //for debugging consumables:
    item a = item(PLUTO);
    item b = item(MERCURY);
    item c = item(PLANET_X);

    consumables.push_back(a);
    consumables.push_back(b);
    consumables.push_back(c);

}

void game::initHandTable(){
    for (int i = 0; i < HAND_TYPE_COUNT; i++)
    {
        for (int j = 0; j < 4; j++){
            handTable[i][j] = atoi(readcsv("handtable.csv", i, j));
        }
    }
    
}

int game::getPlays() {
    return 3 + vouchers[GRABBER] + vouchers[NACHO_TONG];
}

int game::getDiscards() {
    return 4 + vouchers[WASTEFUL] + vouchers[RECYCLOMANCY];
}
