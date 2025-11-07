#include "level.h"

#include <ncursesw/ncurses.h>
#include <cstdlib>
#include "window.h"
#include "game.h"
#include "hand.h"
#include "card.h"

level::level(game* g) {
    this->g = g;
    plays = g->getPlays();
    discards = g->getDiscards();
    d = deck(g->d);
    d.shuffle();
    currentScore = 0;
    recentScore = 0;

    int antebases[9] = {100, 300, 800, 2000, 5000, 11000, 20000, 35000, 50000};
    threshold = antebases[g->ante] * (g->round+1)/2;

    for (int i = 0; i < 8; i++) draw();

    play();
}

void level::play() {
    g->mainScreen.changeTitle("");
    g->levelInfo.updateLevelInfo(this);
    g->gameInfo.updateGameInfo(g);

    g->mainScreen.updateLevelScreen(this);
    g->jokerScreen.updateJokerScreen(g,-1);
    g->specialScreen.updateSpecialScreen(g,0);
    g->cardInfo.updateCardInfo(g,0, static_cast<int>(focusScreen));
    g->peekScreen.updatePeekScreen(this);

    while(true) {
        switch (getchar()) {
            case 'q': // quit
                endwin();
                exit(0);
                return;
            case 'h': // left (vim)
                h.moveCursor(-1);
                break;
            case 'l': // right (vim)
                h.moveCursor(1);
                break;
            case ' ': // select
                h.selectCursor();
                break;
            case 'p': // play
                playHand();
                g->levelInfo.updateLevelInfo(this);
                g->gameInfo.updateGameInfo(g);
                g->peekScreen.updatePeekScreen(this);
                break;
            case 'd': // discard
                discardHand();
                g->levelInfo.updateLevelInfo(this);
                g->gameInfo.updateGameInfo(g);
                g->peekScreen.updatePeekScreen(this);
                break;
            case 's': // swap
                h.swapSelected();
                break;
            case 'z': // sort by suit
                h.sortBySuit();
                break;
            case 'x': // sort by value
                h.sortByValue();
                break;
            case 'j':
                if (focusScreen == CONSUMABLE_SCREEN)
                    changeConsumable(1);
                else
                    changeJoker(1);
                break;
            case 'k':
                if (focusScreen == CONSUMABLE_SCREEN)
                    changeConsumable(-1);
                else
                    changeJoker(-1);
                break;
            case ';':
                swapFocus();
                break;
            case 'w':
                currentScore = threshold;
                break;
        }
        
        g->mainScreen.updateLevelScreen(this);

        if (currentScore >= threshold) {
            win();
            break;
        }
        
        if (plays ==  0) {
            lose();
            break;
        }
    }
}

void level::draw() {
    if(d.cards.empty()) return;
    h.add(d.cards.back());
    d.cards.pop_back();
}

void level::playHand() {
    if (plays == 0) return;
    if (h.cardsSelected() == 0){
        recentScore = 0;
        return;
    } 
    played = hand(h.popSelected());
    for (int i = 0; i < played.selected.size(); i++) draw();
    recentScore = calculateScore(&played); //add score of hand
    plays--;
}

void level::discardHand() {
    if (discards == 0) return;
    recentScore = 0;
    if (h.cardsSelected() == 0) return;
    played = hand(h.popSelected());
    played.cursor = -1;
    for (int i = 0; i < played.selected.size(); i++) draw();
    played = hand(); // "play" an empty hand
    discards--;
}

void level::changeConsumable(int by){
    int newConsumable = currConsumable + by;
    int consSize = g->consumables.size()-1;
    if (newConsumable > consSize || newConsumable < 0) // if player attempt to move outside of bounds
        return;
    
    currConsumable = newConsumable;
    g->specialScreen.updateSpecialScreen(g, currConsumable);
}

void level:: changeJoker(int by){
    int newJokerInd = currJoker + by;
    int jokersSize = g->jokers.size()-1;
    if (newJokerInd > jokersSize || newJokerInd < 0)
        return;
    
    currJoker = newJokerInd;
    g->jokerScreen.updateJokerScreen(g, currJoker);
}

// swaps the controlled screen
void level::swapFocus(){
    if (focusScreen == CONSUMABLE_SCREEN){ // swap to joker screen
        focusScreen = JOKER_SCREEN;
        g->specialScreen.updateSpecialScreen(g, -1); // hide cursor on special screen
        g->jokerScreen.updateJokerScreen(g,0);
        currJoker = 0;
    }
    else{ // swap to consumable screen
        focusScreen = CONSUMABLE_SCREEN;
        g->jokerScreen.updateJokerScreen(g, -1);
        g->specialScreen.updateSpecialScreen(g,0);
        currConsumable = 0;
    }
}

int level::calculateScore(hand* played){
    std::vector<card> scoredCards = played->scoreCards();
    handtype type = played->pokerHand;
    int flat = g->handTable[type][FLAT];
    int mult = g->handTable[type][MULT];
    g->handTable[type][TIMES_PLAYED] += 1;
    int score = flat;

    for (int i = 0; i < scoredCards.size(); i++)
    {
        score += scoredCards[i].cardValue;
    }

    score *= mult;
    currentScore += score;

    return score;
}

void level::win() {
    // earn round money
    int handmoney = plays;
    int interest = g->money / 5;
    int roundreward = g->round + 2;
    int earnings = handmoney + interest + roundreward;

    // display win popup
    window winPopup = window(8, 30, 2, 35, "");
    winPopup.print("Cash Out: $%d\nBlind Reward: $%d\n", earnings, roundreward);
    if(handmoney > 0) winPopup.print("Remaining Hands: $%d\n", handmoney);
    if(interest > 0) winPopup.print("Interest: $%d\n", interest);
    wrefresh(winPopup.content);
    getchar();
    delwin(winPopup.content);
    delwin(winPopup.frame);

    // add money on cash out
    g->money += earnings;
    g->gameInfo.updateGameInfo(g);
}

void level::lose() {
    window lossPopup = window(4, 40, 8, 30, "");
    lossPopup.print("YOU LOSE (you ran out of hands)\nPress any key to quit\n");
    wrefresh(lossPopup.content);
    getchar();
    delwin(lossPopup.content);
    delwin(lossPopup.frame);
}
