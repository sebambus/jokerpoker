#include "shop.h"

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <ncursesw/ncurses.h>
#include "game.h"
#include "item.h"

shop::shop(game *game){
    g = game;
    //if(g->round == 1)
        generatePurchaseables();
    run();
}

void shop::run(){
    g->mainScreen.changeTitle("Shop");
    g->mainScreen.updateShopScreen(this);

    while(true) {
        g->mainScreen.updateShopScreen(this);
        g->gameInfo.updateGameInfo(g);
        g->specialScreen.updateSpecialScreen(g, 0);

        char c = getchar();

        if(c >= 'a' && c <= 'z') {
            int n = c - 'a';

            if(n < items.size()) { // if item selected
                if(g->money >= items[n].cost) {
                    g->consumables.push_back(items[n]);
                    items.erase(items.begin()+n);
                    g->money -= items[n].cost;
                }
                continue;
            } else n-= items.size();
            if(n == 0 && v != VOUCHER_COUNT) {
                if(g->money >= 10) {
                    g->vouchers[v] = true;
                    v = VOUCHER_COUNT;
                    g->money -= 10;
                }
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

void shop::generatePurchaseables() {
    auto genv = [](game* g, auto t) {
        if(g->vouchers.all()) return VOUCHER_COUNT;
        voucher v = (voucher) (rand() % (VOUCHER_COUNT)/2);
        if(g->vouchers[v]) v = (voucher) (v + VOUCHER_COUNT/2);
        if(g->vouchers[v]) v = t(g, t);
        return v;
    };
    v = genv(g, genv);

    for(int i = 0; i < 3; i++) {
        int jokerodds = 20;
        int tarotodds = 4;
        int planetodds = 4;
        int cardodds = 0;
        int n = rand() % (jokerodds + tarotodds + planetodds + cardodds);
        if(n < jokerodds) {
            items.push_back(item((joker) (rand()%JOKER_COUNT)));
            continue;
        } else n -= jokerodds;
        if(n < tarotodds) {
            items.push_back(item((tarot) (rand()%TAROT_COUNT)));
            continue;
        } else n -= tarotodds;
        if(n < planetodds) {
            items.push_back(item((planet) (rand()%PLANET_COUNT)));
            continue;
        } else n -= planetodds;
        if(n < cardodds) {
            items.push_back(item(card(rand()%13, (suit) (rand()%4))));
            continue;
        } else n -= cardodds;
    }
}
