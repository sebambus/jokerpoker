#include "game.h"

#include "level.h"
#include "window.h"
#include "item.h"
#include "readcsv.h"
#include "shop.h"
#include "debug.h"

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

bool running;

void game::run() {
    for(ante = 1; ante <= 8; ante++) {
        s = new shop(this);
        for(round = 1; round <= 3; round++) {
            for(phase = SHOP_PHASE; phase != PHASE_COUNT; phase++) {
                if(phase == SHOP_PHASE && ante+round == 2) continue;
                runinit();
                running = true;
                while(running) {
                    runswitch();
                    runupdate();
                }
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
}

void game::runswitch() {
    char c = getchar();
    universalInput(c);
    if (phase == SHOP_PHASE) shopInput(c);
    if (phase == LEVEL_PHASE) levelInput(c);
}

void game::universalInput(char c){
    switch (c) {
        case 'q': // quit
            endwin();
            exit(0);
        case ';':
            swapFocus();
            break;
        case '2':
            changeFocus(CONSUMABLE_SCREEN);
            break;
        case '3':
            changeFocus(JOKER_SCREEN);
            break;
        case 'j':
            moveMenuCursor(1);
            break;
        case 'k':
            moveMenuCursor(-1);
            break;
    }
 
}

void game::shopInput(char c){
    switch(c) {
    case 'b': // buy item from shop / obtain from pack
        if (focusScreen != MAIN_SCREEN) break;
        useShopItem(currShopItem);
        if (currShopItem > s->shopSize() - 1) currShopItem--; //if item bought was the last (positionally), move cursor back
        break;
    case 'R':
        if(spend(5 + s->rerollCount - vouchers[REROLL_SURPLUS] - vouchers[REROLL_GLUT])) {
            s->reroll();
            s->rerollCount++;
        }
        break;
    case 'C':
        if (s->mode == PACK_MODE){ // if in pack mode, close pack
            s->closePack();
            mainScreen.changeTitle("Shop");
            playingCardInfo.clear();
        } 
        else running = false; // if not, close shop
        break;
    case '1':
        changeFocus(MAIN_SCREEN);
        break;
    
    // the rest of these are for selecting cards inside of pack screens where that's a thing
    case 'h':
        if (s->mode == PACK_MODE){
            s->modifyableCards.moveCursor(-1);
            playingCardInfo.update(s->modifyableCards.cursor);
        }
        break;
    case 'l':
        if (s->mode == PACK_MODE){
            s->modifyableCards.moveCursor(1);
            playingCardInfo.update(s->modifyableCards.cursor);
        }
        break;
    case ' ':
        if (s->mode == PACK_MODE)
            s->modifyableCards.selectCursor();
        break;
    }
}

void game::levelInput(char c){
    switch (c) {
        case 'h': // left (vim)
            l->h.moveCursor(-1);
            playingCardInfo.update(l->h.cursor);
            break;
        case 'l': // right (vim)
            l->h.moveCursor(1);
            playingCardInfo.update(l->h.cursor);
            break;
        case ' ': // select
            l->h.selectCursor();
            break;
        case 'p': // play
            l->playHand();
            levelInfo.update(0);
            gameInfo.update(0);
            peekScreen.update(0);
            break;
        case 'd': // discard
            l->discardHand();
            levelInfo.update(0);
            gameInfo.update(0);
            peekScreen.update(0);
            break;
        case 's': // swap
            l->h.swapSelected();
            break;
        case 'z': // sort by suit
            l->h.sortBySuit();
            break;
        case 'x': // sort by value
            l->h.sortByValue();
            break;
        case 'w':
            l->currentScore = l->threshold;
            break;
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
    return 3 + vouchers[GRABBER] + vouchers[NACHO_TONG];
}

int game::getDiscards() {
    return 4 + vouchers[WASTEFUL] + vouchers[RECYCLOMANCY];
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
