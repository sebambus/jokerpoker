#include "game.h"

#include "level.h"
#include "window.h"
#include "item.h"
#include "readcsv.h"

game::game() :
    levelInfo(10, 20, 0, 0, "Level Info"),
    gameInfo(10, 20, 10, 0, "Game Info"),
    mainScreen(12, 80, 0, 20, ""),
    specialScreen(10, 20, 0, 100, "Consumables"),
    jokerScreen(10, 20, 0, 120, "Jokers"),
    cardInfo(10, 20, 10, 100, "Card Info"),
    peekScreen(8, 80, 12, 20, "Peek") {
    money = 0;
    ante = 1;
    round = 1;
    d.fillDeck();
    initHandTable();
}

int game::getPlays() {
    return 3 + vouchers[GRABBER] + vouchers[NACHO_TONG];
}

int game::getDiscards() {
    return 4 + vouchers[WASTEFUL] + vouchers[RECYCLOMANCY];
}

void game::initHandTable(){
    for (int i = 0; i < HAND_TYPE_COUNT; i++)
    {
        for (int j = 0; j < 4; j++){
            handTable[i][j] = atoi(readcsv("handtable.csv", i, j));
        }
    }
    
}

void game::gain(item i) {
    switch(i.type) {
    case PLANET:
    case TAROT:
    case SPECTRAL:
        consumables.push_back(i);
        break;
    case JOKER:
        jokers.push_back(i);
        break;
    case VOUCHER:
        vouchers[i.val] = true;
        break;
    case CARD:
        d.cards.push_back(card(i));
    }
}

bool game::spend(int x) {
    if(x > money)
        return false;
    else money -= x;
    return true;
}
