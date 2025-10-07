#include "level.h"

#include <ncursesw/ncurses.h>
#include "window.h"

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

void level::play() {
    while(true) {
        clear();
        printWindowedLevel();
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

        if (tally.currentScore >= threshold) {
            clear();
            printw("YOU WIN (you scored over %d)\nPress any key to quit\n", threshold);
            printLevel();
            refresh();
            getchar();
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

    printw("h - Move cursor left\n");
    printw("l - Move cursor right\n");
    printw("Space - Select card\n");
    printw("p - Play hand\n");
    printw("d - Discard selected\n");
    printw("s - Swap selected\n");
    printw("z - sort by suit\n");
    printw("x - Sort by value\n");
}

void level::printWindowedLevel() {
    window levelInfo = window(10, 20, 0, 0, "Level Info");
    window gameInfo = window(10, 20, 10, 0, "Game Info");

    levelInfo.update(this);
    gameInfo.update(g);

    move(2, 25);
    h.print();
    move(5, 25);
    played.print();
    refresh();
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