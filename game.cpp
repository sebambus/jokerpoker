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
    peekScreen(8, 80, 12, 20, "Peek", this, PEEK_SCREEN) {
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
    gameInfo.updateGameInfo();

    if(phase == SHOP_PHASE) {
        s->reopen();
        mainScreen.changeTitle("Shop");
        changeFocus(MAIN_SCREEN);
    }
    if(phase == LEVEL_PHASE) {
        l = new level(this);

        mainScreen.changeTitle("");
        levelInfo.updateLevelInfo();
    
        mainScreen.updateLevelScreen();
        peekScreen.updatePeekScreen();

        if (consumables.size() == 0) // switch to whichever has items
            changeFocus(JOKER_SCREEN);
        else   
            changeFocus(CONSUMABLE_SCREEN);
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
    shopItem si;
    if (s->mode == DEFAULT_MODE) si = s->shopItems[currShopItem];
    else si = s->packItems[currShopItem];

    switch(c) {
    case 'b': // buy item from shop / obtain from pack
        if (focusScreen != MAIN_SCREEN) break;
        if (si.typeOfItem == SI_ITEM){ // item
            if (spend(si.cost)){
                if (s->mode == DEFAULT_MODE){
                    s->shopItems.erase(s->shopItems.begin()+currShopItem);
                    gain(si.i);
                } else { // gain item from pack, needs to redone because only certain packs allow you to obtain the item
                    s->packItems.erase(s->packItems.begin()+currShopItem);
                    gain(si.i);
                    s->packUsesLeft--;
                    if (s->packUsesLeft == 0){
                        s->closePack();
                        currShopItem = 0;
                    }
                }
            }
            return;
        } else if (si.typeOfItem == SI_PACK){ // pack
            if (spend(si.cost)){
                s->open(si.p);
                mainScreen.changeTitle(name(si.p).c_str());
                s->shopItems.erase(s->shopItems.begin()+currShopItem);
                currShopItem = 0;
                s->mode = PACK_MODE;
            }
            return;
        } else if (si.typeOfItem == SI_VOUCHER){ // voucher
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
        if (s->mode == PACK_MODE) s->closePack();
        else running = false;
        break;
    case '1':
        changeFocus(MAIN_SCREEN);
        break;
    case 'h':
        if (s->mode == PACK_MODE)
            s->modifyableCards.moveCursor(-1);
        break;
    case 'l':
        if (s->mode == PACK_MODE)
            s->modifyableCards.moveCursor(1);
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
            break;
        case 'l': // right (vim)
            l->h.moveCursor(1);
            break;
        case ' ': // select
            l->h.selectCursor();
            break;
        case 'p': // play
            l->playHand();
            levelInfo.updateLevelInfo();
            gameInfo.updateGameInfo();
            peekScreen.updatePeekScreen();
            break;
        case 'd': // discard
            l->discardHand();
            levelInfo.updateLevelInfo();
            gameInfo.updateGameInfo();
            peekScreen.updatePeekScreen();
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
        gameInfo.updateGameInfo();
        updateMenuScreens();
    }
    if(phase == LEVEL_PHASE) {
        mainScreen.updateLevelScreen();
    
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
        jokerScreen.updateJokerScreen(-1);
        specialScreen.updateSpecialScreen(currConsumable);
        mainScreen.updateShopScreen(-1);
    }
    else if (focusScreen == JOKER_SCREEN) {
        specialScreen.updateSpecialScreen(-1);
        jokerScreen.updateJokerScreen(currJoker);
        mainScreen.updateShopScreen(-1);
    }
    else if (focusScreen == MAIN_SCREEN){
        specialScreen.updateSpecialScreen(-1);
        jokerScreen.updateJokerScreen(-1);
        mainScreen.updateShopScreen(currShopItem);
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
    
    currConsumable = newConsumable;
    specialScreen.updateSpecialScreen(currConsumable);
}

void game::changeJoker(int by){
    int newJokerInd = currJoker + by;
    int jokersSize = jokers.size()-1;
    if (newJokerInd > jokersSize || newJokerInd < 0)
        return;
    
    currJoker = newJokerInd;
    jokerScreen.updateJokerScreen(currJoker);
}

void game::changeShopItem(int by){
    int newShopItemInd = currShopItem + by;
    int shopItemSize = 0;
    if (s->mode == DEFAULT_MODE)
        shopItemSize = s->shopItems.size();
    else 
        shopItemSize = s->packItems.size();
    if (newShopItemInd > shopItemSize-1 || newShopItemInd < 0)
        return;

    currShopItem = newShopItemInd;
    mainScreen.updateShopScreen(currShopItem);
}

void game::changeFocus(screentype scr){
    if (scr == MAIN_SCREEN){
        if (s->shopItems.size() == 0) return;
        focusScreen = MAIN_SCREEN;
        specialScreen.updateSpecialScreen(-1);
        jokerScreen.updateJokerScreen(-1);
        currShopItem = 0;
        mainScreen.updateShopScreen(0);
    }

    if (scr == JOKER_SCREEN){
        if (jokers.size() == 0) return;
        focusScreen = JOKER_SCREEN;
        specialScreen.updateSpecialScreen(-1);
        mainScreen.updateShopScreen(-1);
        currJoker = 0;
        jokerScreen.updateJokerScreen(0);
    }

    if (scr == CONSUMABLE_SCREEN){
        if (consumables.size() == 0) return;
        focusScreen = CONSUMABLE_SCREEN;
        mainScreen.updateShopScreen(-1);
        jokerScreen.updateJokerScreen(-1);
        currConsumable = 0;
        specialScreen.updateSpecialScreen(0);
    }
}

// swaps the controlled screen
void game::swapFocus(){
    if (focusScreen == CONSUMABLE_SCREEN){ // swap to joker screen
        if (jokers.size() == 0) return;
        focusScreen = JOKER_SCREEN;
        specialScreen.updateSpecialScreen(-1); // hide cursor on special screen
        currJoker = 0;
        jokerScreen.updateJokerScreen(0);
    }
    else{ // swap to consumable screen
        if (consumables.size() == 0) return;
        focusScreen = CONSUMABLE_SCREEN;
        jokerScreen.updateJokerScreen(-1);
        currConsumable = 0;
        specialScreen.updateSpecialScreen(0);
    }
}
