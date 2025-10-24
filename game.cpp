#include "game.h"

#include "level.h"
#include "window.h"
#include "consumable.h"

game::game() :
    levelInfo(10, 20, 0, 0, "Level Info"),
    gameInfo(10, 20, 10, 0, "Game Info"),
    mainScreen(20, 80, 0, 20, ""),
    specialScreen(10, 20, 0, 100, "Jokers and Tarot"),
    cardInfo(10, 20, 10, 100, "Card Info") {
    money = 0;
    ante = 1;
    round = 1;
    d.fillDeck();

    //for debugging:
    consumable a("a");
    a.description = "this is consumable a";
    consumable b("b");
    b.description = "description for consumable b";
    consumable c("c");
    c.description = "this one is consumable c";

    consumables.push_back(a);
    consumables.push_back(b);
    consumables.push_back(c);

}

int game::getPlays() {
    return 3 + vouchers[GRABBER] + vouchers[NACHO_TONG];
}

int game::getDiscards() {
    return 4 + vouchers[WASTEFUL] + vouchers[RECYCLOMANCY];
}
