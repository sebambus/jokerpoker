#include "game.h"

#include "level.h"
#include "window.h"

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
}

int game::getPlays() {
    return 3 + vouchers[GRABBER] + vouchers[NACHO_TONG];
}

int game::getDiscards() {
    return 4 + vouchers[WASTEFUL] + vouchers[RECYCLOMANCY];
}

void game::updateInfo(window w) {
    werase(w.content);
    w.print("You start with\n");
    w.print("Hands Discards\n");
    w.print("  %d      %d\n", getPlays(), getDiscards());
    w.print("Money: $%d\n", money);
    w.print("Ante %d/%d, Round %d\n", ante, 8, round);
    wrefresh(w.content);
}
