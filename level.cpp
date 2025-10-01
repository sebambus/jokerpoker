#include "level.h"

#include <ncursesw/ncurses.h>

void level::setupLevel() {
    d.fillDeck();
    d.shuffle();
    discards = 4;
    plays = 3;
    threshold = 300;
    recentScore = 0;

    for (int i = 0; i < 8; i++) {
        h.add(d.cards.back());
        d.cards.pop_back();
    }
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
