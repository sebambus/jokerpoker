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
    peekScreen(8, 80, 12, 20, "Peek") {
    money = 0;
    d.fillDeck();
    initHandTable();
}

enum phase {
    SHOP_PHASE,
    LEVEL_PHASE,
    PHASE_COUNT
};

int p;
bool running;
shop s(new game());
level l(new game());

void game::run() {
    for(ante = 1; ante <= 8; ante++) {
        s = shop(s);
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
    jokerScreen.updateJokerScreen(this,-1);
    specialScreen.updateSpecialScreen(this,0);

    if(p == SHOP_PHASE) {
        s.reopen();
        mainScreen.changeTitle("Shop");
        mainScreen.updateShopScreen(&s);
    }
    if(p == LEVEL_PHASE) {
        l = level(this);

        mainScreen.changeTitle("");
        levelInfo.updateLevelInfo(&l);
    
        mainScreen.updateLevelScreen(&l);
        peekScreen.updatePeekScreen(&l);
        cardInfo.updateCardInfo(this,0, static_cast<int>(l.focusScreen));
    }
}

void game::runswitch() {
    char c = getchar();

    switch (c) {
    case 'q': // quit
        endwin();
        exit(0);
    }

    if(p == SHOP_PHASE) {
        int n;
        switch(c) {
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
            n = c - 'a';

            if(n < s.items.size()) { // if item selected
                if(spend(s.items[n].cost)) {
                    gain(s.items[n]);
                    s.items.erase(s.items.begin()+n);
                }
                return;
            } else n-= s.items.size();
            if(n < s.packs.size()) {
                if(spend(2*(s.packs[n].size+2)))
                    s.open(s.packs[n]);
                    s.packs.erase(s.packs.begin()+n);
                return;
            } else n-= s.packs.size();
            if(n == 0 && s.v != VOUCHER_COUNT) {
                if(spend(item(s.v).cost)) {
                    gain(item(s.v));
                    s.v = VOUCHER_COUNT;
                }
                return;
            }
            break;
        case 'R':
            if(spend(5 + s.rerollCount - vouchers[REROLL_SURPLUS] - vouchers[REROLL_GLUT])) {
                s.reroll();
                s.rerollCount++;
            }
            break;
        case 'C':
            running = false;
            break;
        }
    }
    if(p == LEVEL_PHASE) {
        switch (c) {
        case 'h': // left (vim)
            l.h.moveCursor(-1);
            break;
        case 'l': // right (vim)
            l.h.moveCursor(1);
            break;
        case ' ': // select
            l.h.selectCursor();
            break;
        case 'p': // play
            l.playHand();
            levelInfo.updateLevelInfo(&l);
            gameInfo.updateGameInfo(this);
            peekScreen.updatePeekScreen(&l);
            break;
        case 'd': // discard
            l.discardHand();
            levelInfo.updateLevelInfo(&l);
            gameInfo.updateGameInfo(this);
            peekScreen.updatePeekScreen(&l);
            break;
        case 's': // swap
            l.h.swapSelected();
            break;
        case 'z': // sort by suit
            l.h.sortBySuit();
            break;
        case 'x': // sort by value
            l.h.sortByValue();
            break;
        case 'j':
            if (l.focusScreen == CONSUMABLE_SCREEN)
                l.changeConsumable(1);
            else
                l.changeJoker(1);
            break;
        case 'k':
            if (l.focusScreen == CONSUMABLE_SCREEN)
                l.changeConsumable(-1);
            else
                l.changeJoker(-1);
            break;
        case ';':
            l.swapFocus();
            break;
        case 'w':
            l.currentScore = l.threshold;
            break;
        }
    }
}

void game::runupdate() {
    if(p == SHOP_PHASE) {
        mainScreen.updateShopScreen(&s);
        specialScreen.updateSpecialScreen(this, 0);
        jokerScreen.updateJokerScreen(this,0);
    }
    if(p == LEVEL_PHASE) {
        mainScreen.updateLevelScreen(&l);
    
        if (l.currentScore >= l.threshold) {
            l.win();
            running = false;
            return;
        }
        
        if (l.plays ==  0) {
            l.lose();
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
