#include "level.h"

#include <ncursesw/ncurses.h>

void level::play() {
    while(true) {
        clear();
        printLevel();
        refresh();

        switch (getchar()) {
            case 'q': // quit
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
                break;
            case 'd': // discard
                discardHand();
                break;
        }
        
        if (plays ==  0) {
            clear();
            printw("YOU LOSE (you ran out of hands)\nPress any key to quit\n");
            printLevel();
            refresh();
            getchar();
            break;
        }

        if (tally.currentScore > threshold) {
            clear();
            printw("YOU WIN (you scored over %d)\nPress any key to quit\n", threshold);
            printLevel();
            refresh();
            getchar();
            break;
        }
    }
}

void level::setupLevel()
{
    d.fillDeck();
    d.shuffle();
    discards = 4;
    plays = 3;
    threshold = 300;
    recentScore = 0;

    for (int i = 0; i < 8; i++) draw();
}

void level::printLevel() {
    printw("Score: %d\n", tally.currentScore);
    printw("Plays: %d, Discards %d\n", plays, discards);
    h.print();
    if(recentScore > 0) {
        printw("%s\n", handName(played.scoreType()));
        printw("+%d Score\n", recentScore);
    }
    played.print();
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
