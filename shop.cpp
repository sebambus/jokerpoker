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
                if(g->spend(items[n].cost)) {
                    g->gain(items[n]);
                    items.erase(items.begin()+n);
                }
                continue;
            } else n-= items.size();
            if(n < packs.size()) {
                if(g->spend(2*(packs[n].size+2)))
                    open(packs[n]);
                    items.erase(items.begin()+n);
                continue;
            } else n-= packs.size();
            if(n == 0 && v != VOUCHER_COUNT) {
                if(g->spend(item(v).cost)) {
                    g->gain(item(v));
                    v = VOUCHER_COUNT;
                }
                continue;
            }
        }

        if(c == 'R' && g->spend(5 + rerollCount - g->vouchers[REROLL_SURPLUS] - g->vouchers[REROLL_GLUT])) {
            reroll();
            rerollCount++;
        }
        if(c == 'C') break;
        if(c == 'q') {
            endwin();
            exit(0);
        }
    }
}

item shop::generateItem(itemtype i) {
    switch (i)  {
    case PLANET:
        return item((planet) (rand()%PLANET_COUNT));
    case TAROT:
        return item((tarot) (rand()%TAROT_COUNT));
    case SPECTRAL:
        return item((spectral) (rand()%SPECTRAL_COUNT));
    case JOKER:
        return item((joker) (rand()%JOKER_COUNT));
    case VOUCHER:
        return item(generateVoucher());
    case CARD:
        return item(card(rand()%13, (suit) (rand()%4)));
    }
}

item shop::generateItem() {
    int odds[TYPE_COUNT] = {20+28*g->vouchers[PLANET_MERCHANT]+112*g->vouchers[PLANET_TYCOON],
                            20+28*g->vouchers[TAROT_MERCHANT]+112*g->vouchers[TAROT_TYCOON],
                            0, 100, 0, 4*g->vouchers[MAGIC_TRICK]};
    int w = 0;
    for(int i : odds) w += i;
    int n = rand() % w;
    for(itemtype i = (itemtype)0; i < TYPE_COUNT; i = (itemtype) (i+1)) {
        if(n < odds[i])
            return generateItem(i);
        else n -= odds[i];
    }
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

pack shop::generatePack() {
    int n = rand();
    itemtype i;
    int s;
    if(n%69 < 20)
        i = CARD;
    else if(n%69 < 40)
        i = TAROT;
    else if(n%69 < 60)
        i = PLANET;
    else if(n%69 < 66)
        i = JOKER;
    else i = SPECTRAL;
    if(n/69%13 < 8)
        s = 0;
    else if(n/69%13 < 12)
        s = 1;
    else s = 2;
    return {i, s};
}

void shop::open(pack p) {
    int n = 2;
    int x = 1;
    if(p.size != 0) n += 2;
    if(p.size == 2) x++;   
    std::vector<item> contents;
    while(contents.size() < n)
        contents.push_back(generateItem(p.type));

    window packPopup = window(2+n, 20, 5-n/2, 50, "PACK");
    while(x > 0) {
        for(char c = 'a'; c < 'a' + contents.size(); c++)
            packPopup.print("%c - %s\n", c, contents[c-'a'].name());
        wrefresh(packPopup.content);

        char c = getchar();
        switch (c) {
        case 'C':
            return;
        case 'q':
            endwin();
            exit(0);
        case 'a':
        case 'b':
        case 'c':
        case 'd':
            g->gain(contents[c-'a']);
            contents.erase(contents.begin()+c-'a');
            x--;
        }
    }
}

void shop::reroll() {
    items.clear();
    while(items.size() < 3)
        items.push_back(generateItem());
}

void shop::reopen() {
    reroll();
    packs.clear();
    while(packs.size() < 2)
        packs.push_back(generatePack());
    rerollCount = 0;
}
