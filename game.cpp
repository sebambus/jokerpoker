#include "game.h"

#include "level.h"
#include "window.h"
#include "item.h"
#include "readcsv.h"
#include "shop.h"
#include "debug.h"
#include "blind.h"

game::game() :
    levelInfo(10, 20, 0, 0, "Level Info", this, LEVEL_INFO_SCREEN),
    gameInfo(10, 20, 10, 0, "Game Info", this, GAME_INFO_SCREEN),
    mainScreen(12, 80, 0, 20, "", this, MAIN_SCREEN),
    specialScreen(10, 20, 0, 100, "Consumables", this, CONSUMABLE_SCREEN),
    jokerScreen(10, 20, 0, 120, "Jokers", this, JOKER_SCREEN),
    cardInfo(10, 20, 10, 100, "Card Info", this, CARD_INFO_SCREEN),
    peekScreen(25, 80, 15, 20, "Peek", this, PEEK_SCREEN),
    playingCardInfo(3,80,12,20, "", this, PLAYING_CARD_INFO_SCREEN){
    s = new shop(this);
    l = new level(this);
    d.fillDeck();
    initHandTable();
}

void game::run() {
    for(ante = 1; ante <= 8; ante++) {
        s = new shop(this);
        makeBlinds();
        for(round = 1; round <= 3; round++) {
            for(phase = SHOP_PHASE; phase != PHASE_COUNT; phase++) {
                if(currentDeck == DECK_TYPE_COUNT) phase = DECK_PHASE; // if deck not selected, select deck
                runinit();
                running = true;
                while(running) {
                    runswitch();
                    runupdate();
                }
                if (phase == DECK_PHASE) phase == LEVEL_PHASE-1; // skip to level phase after deck selection
            }
        }
    }
}

void game::runinit() {
    gameInfo.update(0);

    if(phase == SHOP_PHASE) {
        s->reopen();
        mainScreen.changeTitle("Shop");
        changeFocus(MAIN_SCREEN);
        playingCardInfo.clear();
    }

    if(phase == LEVEL_PHASE) {
        l = new level(this);

        mainScreen.changeTitle("");
        levelInfo.update(0);
    
        mainScreen.update(0);
        peekScreen.update(0);

        if (consumables.size() == 0) // switch to whichever has items
            changeFocus(JOKER_SCREEN);
        else   
            changeFocus(CONSUMABLE_SCREEN);

        playingCardInfo.update(l->h.cursor);
    }

    if (phase == BLINDS_PHASE) {
        mainScreen.changeTitle("Blinds");
        mainScreen.update(0);
        peekScreen.update(0);
    }
}


void game::runupdate() {
    if(phase == SHOP_PHASE) {
        gameInfo.update(0);
        updateMenuScreens();
    }
    if(phase == LEVEL_PHASE) {
        mainScreen.update(0);
        playingCardInfo.update(l->h.cursor);

    
        if (l->currentScore >= l->threshold) {
            l->win();
            running = false;
            return;
        }
        
        if (l->plays ==  0) {
            l->lose();
            running = false;
            return;
        }
    }
}

void game::useShopItem(int currItem){
    // if we are in pack mode
    if (s->mode == PACK_MODE){
        item i = s->packItems[currShopItem];
        s->packItems.erase(s->packItems.begin()+currShopItem);
        itemtype it = i.type;
        switch (it)
        {
        case JOKER:
        case CARD:
            gain(i);
            break;
        // tarots, spectrals, and planets are used, which is not yet implemented
        default:
            break;
        }
        s->closePack();
        currShopItem = 0;
        playingCardInfo.clear();
        return;
    }


    // if item is an item
    if (currItem >= 0 && currItem < s->items.size()){
        item i = s->items[currItem];
        if (spend(i.cost)){
            gain(i);    
            s->items.erase(s->items.begin() + currItem);
        }
    }

    // if item is a pack
    else if (currItem >= s->items.size() && currItem < s->packs.size() + s->items.size()){
        pack p = s->packs[currItem - s->items.size()];
        if (spend(p.cost)){
            s->open(p);
            mainScreen.changeTitle(name(p).c_str());
            currShopItem = 0;
            s->mode = PACK_MODE;
            s->packs.erase(s->packs.begin() + currItem - s->items.size());
            playingCardInfo.update(s->modifyableCards.cursor);
        }
    }

    // if item is a voucher
    else {
        item voucherItem = item(s->v);
        if (spend(voucherItem.cost)){
            gain(voucherItem);
            s->v = VOUCHER_COUNT;
        }
    }
    
}

int game::getPlays() {
    return 3 + vouchers[GRABBER] + vouchers[NACHO_TONG] + currentDeck == BLUE_DECK;
}

int game::getDiscards() {
    return 3 + vouchers[WASTEFUL] + vouchers[RECYCLOMANCY] + currentDeck == RED_DECK;
}

void game::initHandTable(){
    for (int i = 0; i < HAND_TYPE_COUNT; i++)
    {
        for (int j = 1; j < 5; j++){
            handTable[i][j-1] = atoi(readcsv("handtable.csv", i, j));
        }
    }
    
}

void game::gain(item i) {
    switch(i.type) {
    case PLANET:
    case TAROT:
    case SPECTRAL:
        consumables.push_back(i);
        break;
    case JOKER:
        jokers.push_back(i);
        break;
    case VOUCHER:
        vouchers[i.val] = true;
        break;
    case CARD:
        d.cards.push_back(card(i));
        break;
    }
}

bool game::spend(int x) {
    if(x > money)
        return false;
    else money -= x;
    return true;
}

void game::updateMenuScreens(){
    if (focusScreen == CONSUMABLE_SCREEN){
        jokerScreen.update(-1);
        specialScreen.update(currConsumable);
        mainScreen.update(-1);
    }
    else if (focusScreen == JOKER_SCREEN) {
        specialScreen.update(-1);
        jokerScreen.update(currJoker);
        mainScreen.update(-1);
    }
    else if (focusScreen == MAIN_SCREEN){
        specialScreen.update(-1);
        jokerScreen.update(-1);
        mainScreen.update(currShopItem);
    }
}

void game::moveMenuCursor(int by){
    switch(focusScreen){
        case CONSUMABLE_SCREEN:
            changeConsumable(by);
            break;
        case JOKER_SCREEN:
            changeJoker(by);
            break;
        case MAIN_SCREEN:
            changeShopItem(by);
            break;
    }
}


void game::changeConsumable(int by){
    int newConsumable = currConsumable + by;
    int consSize = consumables.size()-1;
    if (newConsumable > consSize || newConsumable < 0) // if player attempt to move outside of bounds
        return;
    
    // if not moving beyond value, show that your have the next consumable selected
    currConsumable = newConsumable;
    specialScreen.update(currConsumable);
}

void game::changeJoker(int by){
    int newJokerInd = currJoker + by;
    int jokersSize = jokers.size()-1;
    if (newJokerInd > jokersSize || newJokerInd < 0)
        return;
    
    currJoker = newJokerInd;
    jokerScreen.update(currJoker);
}

// change selected shop item
void game::changeShopItem(int by){
    int newShopItemInd = currShopItem + by;
    int shopItemSize = 0;
    if (s->mode == DEFAULT_MODE)
        shopItemSize = s->shopSize();
    else 
        shopItemSize = s->packItems.size();
    if (newShopItemInd > shopItemSize -1 || newShopItemInd < 0){
        return;
    }

    currShopItem = newShopItemInd;
    mainScreen.update(currShopItem);
}

void game::changeFocus(screentype scr){
    if (scr == MAIN_SCREEN){
        if (s->shopSize() == 0) return;
        focusScreen = MAIN_SCREEN;
        specialScreen.update(-1);
        jokerScreen.update(-1);
        currShopItem = 0;
        mainScreen.update(0);
    }

    if (scr == JOKER_SCREEN){
        if (jokers.size() == 0) return;
        focusScreen = JOKER_SCREEN;
        specialScreen.update(-1);
        mainScreen.update(-1);
        currJoker = 0;
        jokerScreen.update(0);
    }

    if (scr == CONSUMABLE_SCREEN){
        if (consumables.size() == 0) return;
        focusScreen = CONSUMABLE_SCREEN;
        mainScreen.update(-1);
        jokerScreen.update(-1);
        currConsumable = 0;
        specialScreen.update(0);
    }
}

// swaps the controlled screen
void game::swapFocus(){
    if (focusScreen == CONSUMABLE_SCREEN){ // swap to joker screen
        if (jokers.size() == 0) return;
        focusScreen = JOKER_SCREEN;
        specialScreen.update(-1); // hide cursor on special screen
        currJoker = 0;
        jokerScreen.update(0);
    }
    else{ // swap to consumable screen
        if (consumables.size() == 0) return;
        focusScreen = CONSUMABLE_SCREEN;
        jokerScreen.update(-1);
        currConsumable = 0;
        specialScreen.update(0);
    }
}

void game::makeBlinds(){
    currBlinds[0] = blind(SMALL_BLIND);
    currBlinds[1] = blind(BIG_BLIND);
    currBlinds[2] = blind(ante, completedBossBlinds);
    completedBossBlinds.push_back(currBlinds[2].bossType);
}

void game::skipBlind(){
    tags.push_back(currBlinds[round - 1].skipTag); // gain the skipped blinds tag
    debug(tags[0].name);
    round++;
    runinit();
}