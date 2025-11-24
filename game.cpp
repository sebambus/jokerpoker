#include "game.h"

#include "level.h"
#include "window.h"
#include "item.h"
#include "readcsv.h"
#include "shop.h"

game::game() :
    levelInfo(10, 20, 0, 0, "Level Info"),
    gameInfo(10, 20, 10, 0, "Game Info"),
    mainScreen(12, 80, 0, 20, ""),
    specialScreen(10, 20, 0, 100, "Consumables"),
    jokerScreen(10, 20, 0, 120, "Jokers"),
    cardInfo(10, 20, 10, 100, "Card Info"),
    peekScreen(8, 80, 12, 20, "Peek")
    {
    s = new shop(this);
    l = new level(this);
    money = 0;
    d.fillDeck();
    initHandTable();
}



int p;
bool running;

void game::run() {
    for(ante = 1; ante <= 8; ante++) {
        s = new shop(this);
        for(round = 1; round <= 3; round++) {
            for(p = SHOP_PHASE; p != PHASE_COUNT; p++) {
                if(p == SHOP_PHASE && ante+round == 2) continue;
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
    gameInfo.updateGameInfo(this);

    if(p == SHOP_PHASE) {
        s->reopen();
        mainScreen.changeTitle("Shop");
        changeFocus(SHOP_SCREEN);
    }
    if(p == LEVEL_PHASE) {
        l = new level(this);

        mainScreen.changeTitle("");
        levelInfo.updateLevelInfo(l);
    
        mainScreen.updateLevelScreen(l);
        peekScreen.updatePeekScreen(l);

        if (consumables.size() == 0) // switch to whichever has items
            changeFocus(JOKER_SCREEN);
        else   
            changeFocus(CONSUMABLE_SCREEN);
    }
}

void game::runswitch() {
    char c = getchar();

    switch (c) {
    case 'q': // quit
        endwin();
        exit(0);
    }

    // input loop for the shop
    if(p == SHOP_PHASE) {
        shopItem si = s->shopItems[currShopItem];
        switch(c) {
        case 'b': // buy item from shop
            if (focusScreen != SHOP_SCREEN) break;
            if (si.typeOfItem == 0){ // item
                if (spend(si.cost)){
                    gain(si.i);
                    s->shopItems.erase(s->shopItems.begin()+currShopItem);
                }
                return;
            } else if (si.typeOfItem == 1){ // pack
                if (spend(si.cost)){
                s->open(si.p);
                s->shopItems.erase(s->shopItems.begin()+currShopItem);
                }
                return;
            } else if (si.typeOfItem == 2){ // voucher
                if (spend(si.cost)){
                    gain(si.v);
                    s->shopItems.erase(s->shopItems.begin()+currShopItem);
                }
                return;
            }
            break;
        case 'R':
            if(spend(5 + s->rerollCount - vouchers[REROLL_SURPLUS] - vouchers[REROLL_GLUT])) {
                s->reroll();
                s->rerollCount++;
            }
            break;
        case 'C':
            running = false;
            break;
        case 'j':
            moveMenuCursor(1);
            break;
        case 'k':
            moveMenuCursor(-1);
            break;
        case '-':
            changeShopItem(-1);
            break;
        case '=':
            changeShopItem(1);
            break;
        case '1':
            changeFocus(SHOP_SCREEN);
            break;
        case '2':
            changeFocus(CONSUMABLE_SCREEN);
            break;
        case '3':
            changeFocus(JOKER_SCREEN);
            break;
        }
    }

    // input loop for the gameplay
    if(p == LEVEL_PHASE) {
        switch (c) {
        case 'h': // left (vim)
            l->h.moveCursor(-1);
            break;
        case 'l': // right (vim)
            l->h.moveCursor(1);
            break;
        case ' ': // select
            l->h.selectCursor();
            break;
        case 'p': // play
            l->playHand();
            levelInfo.updateLevelInfo(l);
            gameInfo.updateGameInfo(this);
            peekScreen.updatePeekScreen(l);
            break;
        case 'd': // discard
            l->discardHand();
            levelInfo.updateLevelInfo(l);
            gameInfo.updateGameInfo(this);
            peekScreen.updatePeekScreen(l);
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
        case 'j':
            moveMenuCursor(1);
            break;
        case 'k':
            moveMenuCursor(-1);
            break;
        case '2':
            changeFocus(CONSUMABLE_SCREEN);
            break;
        case '3':
            changeFocus(JOKER_SCREEN);
            break;
        }
    }
}

void game::runupdate() {
    if(p == SHOP_PHASE) {
        gameInfo.updateGameInfo(this);
        updateMenuScreens();
    }
    if(p == LEVEL_PHASE) {
        mainScreen.updateLevelScreen(l);
    
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

int game::getPlays() {
    return 3 + vouchers[GRABBER] + vouchers[NACHO_TONG];
}

int game::getDiscards() {
    return 4 + vouchers[WASTEFUL] + vouchers[RECYCLOMANCY];
}

void game::initHandTable(){
    for (int i = 0; i < HAND_TYPE_COUNT; i++)
    {
        for (int j = 0; j < 4; j++){
            handTable[i][j] = atoi(readcsv("handtable.csv", i, j));
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
        jokerScreen.updateJokerScreen(this,-1);
        specialScreen.updateSpecialScreen(this,currConsumable);
        mainScreen.updateShopScreen(this,-1);
    }
    else if (focusScreen == JOKER_SCREEN) {
        specialScreen.updateSpecialScreen(this,-1);
        jokerScreen.updateJokerScreen(this,currJoker);
        mainScreen.updateShopScreen(this,-1);
    }
    else if (focusScreen = SHOP_SCREEN){
        specialScreen.updateSpecialScreen(this,-1);
        jokerScreen.updateJokerScreen(this,-1);
        mainScreen.updateShopScreen(this,currShopItem);
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
        case SHOP_SCREEN:
            changeShopItem(by);
            break;
    }
}


void game::changeConsumable(int by){
    int newConsumable = currConsumable + by;
    int consSize = consumables.size()-1;
    if (newConsumable > consSize || newConsumable < 0) // if player attempt to move outside of bounds
        return;
    
    currConsumable = newConsumable;
    specialScreen.updateSpecialScreen(this, currConsumable);
}

void game::changeJoker(int by){
    int newJokerInd = currJoker + by;
    int jokersSize = jokers.size()-1;
    if (newJokerInd > jokersSize || newJokerInd < 0)
        return;
    
    currJoker = newJokerInd;
    jokerScreen.updateJokerScreen(this, currJoker);
}

void game::changeShopItem(int by){
    int newShopItemInd = currShopItem + by;
    int shopItemSize = s->shopItems.size() -1;
    if (newShopItemInd > shopItemSize || newShopItemInd < 0)
        return;

    currShopItem = newShopItemInd;
    mainScreen.updateShopScreen(this, currShopItem);
}

void game::changeFocus(selectableScreen scr){
    if (scr == SHOP_SCREEN){
        if (s->shopItems.size() == 0) return;
        focusScreen = SHOP_SCREEN;
        specialScreen.updateSpecialScreen(this,-1);
        jokerScreen.updateJokerScreen(this,-1);
        currShopItem = 0;
        mainScreen.updateShopScreen(this,0);
    }

    if (scr == JOKER_SCREEN){
        if (jokers.size() == 0) return;
        focusScreen = JOKER_SCREEN;
        specialScreen.updateSpecialScreen(this,-1);
        mainScreen.updateShopScreen(this,-1);
        currJoker = 0;
        jokerScreen.updateJokerScreen(this,0);
    }

    if (scr == CONSUMABLE_SCREEN){
        if (consumables.size() == 0) return;
        focusScreen = CONSUMABLE_SCREEN;
        mainScreen.updateShopScreen(this,-1);
        jokerScreen.updateJokerScreen(this,-1);
        currConsumable = 0;
        specialScreen.updateSpecialScreen(this,0);
    }
}

// swaps the controlled screen
void game::swapFocus(){
    if (focusScreen == CONSUMABLE_SCREEN){ // swap to joker screen
        if (jokers.size() == 0) return;
        focusScreen = JOKER_SCREEN;
        specialScreen.updateSpecialScreen(this, -1); // hide cursor on special screen
        currJoker = 0;
        jokerScreen.updateJokerScreen(this,0);
    }
    else{ // swap to consumable screen
        if (consumables.size() == 0) return;
        focusScreen = CONSUMABLE_SCREEN;
        jokerScreen.updateJokerScreen(this, -1);
        currConsumable = 0;
        specialScreen.updateSpecialScreen(this,0);
    }
}
