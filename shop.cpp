#include "shop.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <ncursesw/ncurses.h>
#include "game.h"
#include "item.h"
#include "debug.h"
#include "deck.h"

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

std::string description(pack p){
    int cardsInPack;
    int cardsToChoose;
    switch(p.type){
        case TAROT:
        case PLANET:
        case CARD:
            if (p.size == 0){
                cardsToChoose = 1;
                cardsInPack = 3;
            } 
            else if (p.size == 1){
                cardsToChoose = 1;
                cardsInPack = 5;
            }
            else if (p.size == 2){
                cardsToChoose = 2;
                cardsInPack = 5;                
            }
            break;
        case JOKER:
        case SPECTRAL:
           if (p.size == 0){
                cardsToChoose = 1;
                cardsInPack = 2;
            } 
            else if (p.size == 1){
                cardsToChoose = 1;
                cardsInPack = 4;
            }
            else if (p.size == 2){
                cardsToChoose = 2;
                cardsInPack = 4;                
            }
            break;
    }

    std::string type;
    std::string action;
    switch(p.type){
        case JOKER:
            type = "Joker";
            action = ".";
            break;
        case PLANET:
            type = "Planet";
            action = " to be used immediately.";
            break;
        case CARD:
            type = "Playing";
            action = " to add to your Deck.";
            break;
        case SPECTRAL:
            type = "Spectral";
            action = " to be used immediately.";
            break;
        case TAROT:
            type = "Tarot";
            action = " to be used immediately.";
            break;
    }
    std::string out = "Choose " + std::to_string(cardsToChoose) + " of " + std::to_string(cardsInPack) + " " + type + " Cards" + action;

    return out;
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
        packItems.push_back(si);
    }

    if (p.type == JOKER || p.type == CARD) // you dont modify any cards with joker or card packs
        return;

    g->d.shuffle();

    for (int i = 0; i < 10; i++)
    {
        modifyableCards.add(g->d.cards.back());
        g->d.cards.pop_back();
    }
    
}

void shop::closePack(){
    mode = DEFAULT_MODE;
    for (card c : modifyableCards.cards) // add the cards put in the pack screen back into the deck
        g->d.cards.push_back(c);
    modifyableCards.cards.clear();

}


void shop::reroll() {
    items.clear();
    while(items.size() < 3)
        items.push_back(generateItem());
    fillShopItems();
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
    if (n.type != CARD){ // create it as an item
        typeOfItem = SI_ITEM;
        i = n;
        cost = i.cost;
    }
    else { // create it as a card
        typeOfItem = SI_CARD;
        int cValue = n.val % 13;
        int cSuit = n.val / 13;
        c = card(cValue, (suit) cSuit); 
        cost = 1;
    }
}

shopItem::shopItem(pack n){
    typeOfItem = SI_PACK;
    p = n;
    cost = 2 * (n.size + 2);
}

shopItem::shopItem(voucher n){
    typeOfItem = SI_VOUCHER;
    v = n;
    cost = 10;
}

std::string shopItem::getName(){
    const char* cstring;
    std::string s;
    switch (typeOfItem){
    case SI_ITEM:
        cstring = i.name();
        break;
    case SI_PACK:
        s = name(p);
        break;
    case SI_VOUCHER:
        cstring = item(v).name();
        break;
    case SI_CARD:
        s = c.name();
        break;
    }


    if (!s.empty())
        return s;
    
    std::string name(cstring);    
    return name;
}