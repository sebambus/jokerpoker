#pragma once
#include <string>

// run make once to remove these errors
#include "obj/planet.h"
#include "obj/tarot.h"
#include "obj/spectral.h"
#include "obj/joker.h"
#include "obj/voucher.h"

class card;

enum itemtype : int {
    PLANET,
    TAROT,
    SPECTRAL,
    JOKER,
    VOUCHER,
    CARD
};

class item {
public:
    int val;
    itemtype type;
    int cost;

    item(planet);
    item(tarot);
    item(spectral);
    item(joker);
    item(voucher);
    item(card);
    const char* name();
    const char* description();
};