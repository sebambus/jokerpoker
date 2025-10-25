#include "item.h"

#include "readcsv.h"
#include "card.h"

item::item(planet p) {
    type = PLANET;
    val = p;
}

item::item(tarot t) {
    type = TAROT;
    val = t;
}

item::item(spectral s) {
    type = SPECTRAL;
    val = s;
}

item::item(joker j) {
    type = JOKER;
    val = j;
}

item::item(voucher v) {
    type = VOUCHER;
    val = v;
}

item::item(card c) {
    type = CARD;
    val = c.cardSuit*13 + c.cardValue;
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
    info(this, 0);
}

const char* item::description(){
    info(this, 1);
}