#include "shop.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <ncursesw/ncurses.h>
#include "game.h"
#include "item.h"
#include "debug.h"

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
    packItems.clear();
    int itemsInPack = 3;
    packUsesLeft = 1;
    if(p.size != 0) itemsInPack += 2;
    if(p.size == 2) packUsesLeft++;   
    while (packItems.size() != itemsInPack){
        shopItem si(generateItem(p.type));
        si.cost = 0;
        packItems.push_back(si);
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
    fillShopItems();
    rerollCount = 0;
}

void shop::fillShopItems(){
    shopItems.clear();
    for (item i: items){
        shopItem si(i);
        shopItems.push_back(si);
    }

    for (pack p: packs){
        shopItem si(p);
        shopItems.push_back(si);
    }

    shopItem si(v);
    shopItems.push_back(si);

}

shopItem::shopItem(){

}

shopItem::shopItem(item n){
    typeOfItem = 0;
    i = n;
    cost = i.cost;
}

shopItem::shopItem(pack n){
    typeOfItem = 1;
    p = n;
    cost = 2 * (n.size + 2);
}

shopItem::shopItem(voucher n){
    typeOfItem = 2;
    v = n;
    cost = 10;
}

std::string shopItem::getName(){
    const char* cstring;
    std::string s;
    switch (typeOfItem){
    case 0:
        cstring = i.name();
        break;
    case 1:
        s = name(p);
        break;
    case 2:
        cstring = item(v).name();
        break;
    }

    if (!s.empty())
        return s;
    
    std::string name(cstring);    
    return name;
}