#include "shop.h"

#include <stdio.h>
#include <stdlib.h>
#include <ncursesw/ncurses.h>
#include "game.h"
#include "item.h"

shop::shop(game *game){
    g = game;
    if(g->round == 1)
        generatePurchaseables();
    run();
}

void shop::run(){
    g->mainScreen.changeTitle("Shop");
    g->mainScreen.updateShopScreen(this);
    char c = getchar();
    if(c >= 'a' && c-'a' < v != VOUCHER_COUNT) {
        if(g->money >= 10) {
            g->vouchers[v] = true;
            v = VOUCHER_COUNT;
            g->money -= 10;
        }
    }
    g->mainScreen.updateShopScreen(this);
    g->gameInfo.updateGameInfo(g);
    getchar();
}

void shop::generatePurchaseables() {
    auto genv = [](game* g, auto t) {
        if(g->vouchers.all()) return VOUCHER_COUNT;
        voucher v = (voucher) (rand() % (VOUCHER_COUNT)/2);
        if(g->vouchers[v]) v = (voucher) (v + VOUCHER_COUNT/2);
        if(g->vouchers[v]) v = t(g, t);
        return v;
    };
    v = genv(g, genv);


}
