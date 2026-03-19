#include "game.h"

void game::runswitch() {
    char c = getchar();
    universalInput(c);
    if (phase == SHOP_PHASE) shopInput(c);
    if (phase == LEVEL_PHASE) levelInput(c);
    if (phase == BLINDS_PHASE) blindsInput(c);
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
        case 'P':
            running = false;
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

void game::blindsInput(char c){
    switch (c)
    {
    case 'e':
        running = false;
        break;
    case 's':
        if (round != 3) // only small and big blinds are skippable
            skipBlind();
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
