#include "shop.h"

#include <stdio.h>
#include <stdlib.h>
#include <ncursesw/ncurses.h>
#include "game.h"
#include "voucher.h"

shop::shop(game *game){
    g = game;
    v = (voucher) (rand() % VOUCHER_COUNT);
    run();
}

void shop::run(){
    g->mainScreen.changeTitle("Shop");
    g->mainScreen.updateShopScreen(this);
    getch();
    return;
}