#include "level.h"

#include <ncursesw/ncurses.h>
#include <cstdlib>
#include "window.h"
#include "game.h"
#include "hand.h"
#include "card.h"

level::level(game* g, int threshold) {
    this->g = g;
    plays = g->plays;
    discards = g->discards;
    d = deck(g->d);
    d.shuffle();
    this->threshold = threshold;
    recentScore = 0;

    for (int i = 0; i < 8; i++) draw();
}

void updateGameScreen(WINDOW* win, hand h, hand played) {
    werase(win);
    h.print(win);
    wmove(win, 3, 0);
    played.print(win);
    wrefresh(win);
}

void level::play() {
    window levelInfo = window(10, 20, 0, 0, "Level Info");
    window gameInfo = window(10, 20, 10, 0, "Game Info");
    window gameScreen = window(20, 80, 0, 20, "Game");

    window specialScreen = window(15,20,0,100, "Jokers and Tarot");
    window cardInfo = window(15,20,15,100, "Card Info");

    levelInfo.update(this);
    gameInfo.update(g);

    updateGameScreen(gameScreen.content, h, played);

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
                levelInfo.update(this);
                gameInfo.update(g);
                break;
            case 'd': // discard
                discardHand();
                levelInfo.update(this);
                gameInfo.update(g);
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
        }
        
        updateGameScreen(gameScreen.content, h, played);

        if (tally.currentScore >= threshold) {
            window winPopup = window(4, 40, 8, 30, "");
            winPopup.print("YOU WIN (you scored over %d)\nPress any key to quit\n", threshold);
            wrefresh(winPopup.content);
            getchar();
            levelInfo.clear();
            gameInfo.clear();
            gameScreen.clear();
            break;
        }
        
        if (plays ==  0) {
            window lossPopup = window(4, 40, 8, 30, "");
            lossPopup.print("YOU LOSE (you ran out of hands)\nPress any key to quit\n");
            wrefresh(lossPopup.content);
            getchar();
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
    played.cursor = -1;
    for (int i = 0; i < played.selected.size(); i++) draw();
    recentScore = tally.calculateScore(played); //add score of hand
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