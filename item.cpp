#include "item.h"

#include "readcsv.h"
#include "voucher.h"
#include "card.h"

item::item(voucher v) {
    type = VOUCHER;
    val = v;
}

item::item(planet p) {
    type = PLANET;
    val = p;
}

item::item(card c) {
    type = CARD;
    val = c.cardSuit*13 + c.cardValue;
}

const char* item::name()
{
    switch (type) {
    case PLANET:
        return readcsv("planet.csv", val, 0);
    case VOUCHER:
        return readcsv("voucher.csv", val, 0);
    }
    return "ERROR";
}

const char* item::description(){
    switch (type) {
    case PLANET:
        return readcsv("planet.csv", val, 1);
    }
    return "ERROR";
}