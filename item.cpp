#include "item.h"

#include "readcsv.h"
#include "card.h"
#include "shop.h"

item::item(){
    
}

item::item(itemtype i) {
    switch (i)  {
    case PLANET:
        *this = item((planet) (rand()%PLANET_COUNT));
		break;
    case TAROT:
        *this = item((tarot) (rand()%TAROT_COUNT));
		break;
    case SPECTRAL:
        *this = item((spectral) (rand()%SPECTRAL_COUNT));
		break;
    case JOKER:
        *this = item((joker) (rand()%JOKER_COUNT));
		break;
    case VOUCHER:
        *this = item(shop(nullptr).generateVoucher());
		break;
    case CARD:
        *this = item(card(rand()%13, (suit) (rand()%4)));
		break;
    }
}

item::item(planet p) {
    type = PLANET;
    val = p;
    cost = 3;
}

item::item(tarot t) {
    type = TAROT;
    val = t;
    cost = 3;
}

item::item(spectral s) {
    type = SPECTRAL;
    val = s;
    cost = 4;
}

item::item(joker j) {
    type = JOKER;
    val = j;
    cost = atoi(readcsv("joker.csv", j, 2));
}

item::item(voucher v) {
    type = VOUCHER;
    val = v;
    cost = 10;
}

item::item(card c) {
    type = CARD;
    val = c.cardSuit*13 + c.cardValue;
    cost = 1;
}

static const char* info(item* i, int x) {
    switch (i->type) {
    case PLANET:
        return readcsv("planet.csv", i->val, x);
    case TAROT:
        return readcsv("tarot.csv", i->val, x);
    case SPECTRAL:
        return readcsv("spectral.csv", i->val, x);
    case JOKER:
        return readcsv("joker.csv", i->val, x);
    case VOUCHER:
        return readcsv("voucher.csv", i->val, x);
    }
    return "ERROR";
}

const char* item::name() {
    return info(this, 0);
}

std::string item::description(){
    std::string desc(info(this,1));
    return desc;
}