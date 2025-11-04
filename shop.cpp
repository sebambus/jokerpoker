#include "shop.h"

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <ncursesw/ncurses.h>
#include "game.h"
#include "item.h"

shop::shop(game *game){
    g = game;
    v = generateVoucher();
}

void shop::run(){
    g->mainScreen.changeTitle("Shop");
    g->mainScreen.updateShopScreen(this);

    while(true) {
        g->mainScreen.updateShopScreen(this);
        g->gameInfo.updateGameInfo(g);
        g->specialScreen.updateSpecialScreen(g, 0);
        g->jokerScreen.updateJokerScreen(g,0);

        char c = getchar();

        if(c >= 'a' && c <= 'z') {
            int n = c - 'a';

            if(n < items.size()) { // if item selected
                if(g->buy(items[n]))
                    items.erase(items.begin()+n);
                continue;
            } else n-= items.size();
            if(n == 0 && v != VOUCHER_COUNT) {
                if(g->buy(item(v)))
                    v = VOUCHER_COUNT;
                continue;
            }
        }

        if(c == 'C') break;
        if(c == 'q') {
            endwin();
            exit(0);
        }
    }
}

item shop::generateItem() {
    int jokerodds = 20;
    int tarotodds = 4;
    int planetodds = 4;
    int cardodds = 0;
    int n = rand() % (jokerodds + tarotodds + planetodds + cardodds);
    if(n < jokerodds) {
        return item((joker) (rand()%JOKER_COUNT));
    } else n -= jokerodds;
    if(n < tarotodds) {
        return item((tarot) (rand()%TAROT_COUNT));
    } else n -= tarotodds;
    if(n < planetodds) {
        return item((planet) (rand()%PLANET_COUNT));
    } else n -= planetodds;
    if(n < cardodds) {
        return item(card(rand()%13, (suit) (rand()%4)));
    } else n -= cardodds;
}

voucher shop::generateVoucher() {
    auto genv = [](game* g, auto t) {
        if(g->vouchers.all()) return VOUCHER_COUNT;
        voucher v = (voucher) (rand() % (VOUCHER_COUNT)/2);
        if(g->vouchers[v]) v = (voucher) (v + VOUCHER_COUNT/2);
        if(g->vouchers[v]) v = t(g, t);
        return v;
    };
    return genv(g, genv);
}

void shop::refresh() {
    items.clear();

    while(items.size() < 3)
        items.push_back(generateItem());
}
