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
    g->levelInfo.update(this);
    g->gameInfo.update(g);

    updateGameScreen(g->mainScreen.content, h, played);

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
                g->levelInfo.update(this);
                g->gameInfo.update(g);
                break;
            case 'd': // discard
                discardHand();
                g->levelInfo.update(this);
                g->gameInfo.update(g);
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
        
        updateGameScreen(g->mainScreen.content, h, played);

        if (tally.currentScore >= threshold) {
            window winPopup = window(4, 40, 8, 30, "");
            winPopup.print("YOU WIN (you scored over %d)\nPress any key to quit\n", threshold);
            wrefresh(winPopup.content);
            getchar();
            g->levelInfo.clear();
            g->gameInfo.clear();
            g->mainScreen.clear();
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