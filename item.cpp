#include "item.h"
#include "debug.h"
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
        enhancement randomEnhancement = BASE_CARD;
        seal randomSeal = NO_SEAL;
        if ((rand() % 100) < 40) randomEnhancement = static_cast<enhancement>(rand() % ENHANCEMENT_COUNT);
        if ((rand() % 100) < 20) randomSeal = static_cast<seal>(rand() % SEAL_COUNT);
        // (rand % 13) + 1 so that there is no card with value 1
        *this = item(card((rand()%13)+1, (suit)(rand()%4), randomEnhancement, randomSeal));
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
    val = (((c.cardSuit * 13 + c.cardValue) * ENHANCEMENT_COUNT
        + c.cardEnhancement) * SEAL_COUNT
        + c.cardSeal);
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

std::string item::name() {
    if (type == CARD) {
        std::string n = card(*this).name();
        return n;
    }

    std::string inf = info(this,0);
    return inf;
}

std::string item::description(){
    std::string desc(info(this,1));
    return desc;
}