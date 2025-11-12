#include "shop.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <ncursesw/ncurses.h>
#include "game.h"
#include "item.h"

std::string name(pack p) {
    std::string s = "";
    switch (p.size) {
    case 1:
        s += "Jumbo ";
        break;
    case 2:
        s += "Mega ";
        break;
    }
    switch (p.type) {
    case PLANET:
        s += "Celestial";
        break;
    case TAROT:
        s += "Arcana";
        break;
    case SPECTRAL:
        s += "Spectral";
        break;
    case JOKER:
        s += "Buffoon";
        break;
    case CARD:
        s += "Standard";
        break;
    }
    s += " Pack";
    return s;
}

shop::shop(game *game){
    g = game;
    v = generateVoucher();
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

    window packPopup = window(2+n, 26, 5-n/2, 47, name(p).c_str());
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
