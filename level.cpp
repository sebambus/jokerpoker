#include <ncursesw/ncurses.h>
#include <cstdlib>
#include "window.h"
#include "game.h"
#include "hand.h"
#include "card.h"
#include "scorekeep.h"

static int discards;
static int plays;
static scorekeep tally;
static int recentScore;
static int threshold;
static deck d;
static hand h;
static hand played;

static void draw();
static void playHand();
static void discardHand();
static void updateInfo(window);
static void updateScreen(window);

void game::playlevel() {
    plays = getPlays();
    discards = getDiscards();
    ::d = deck(this->d);
    ::d.shuffle();
    recentScore = 0;

    int antebases[9] = {100, 300, 800, 2000, 5000, 11000, 20000, 35000, 50000};
    threshold = antebases[ante] * (round+1)/2;

    for (int i = 0; i < 8; i++) draw();

    updateInfo(levelInfo);
    updateInfo(gameInfo);

    updateScreen(mainScreen);

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
                updateInfo(levelInfo);
                updateInfo(gameInfo);
                break;
            case 'd': // discard
                discardHand();
                updateInfo(levelInfo);
                updateInfo(gameInfo);
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
        
        updateScreen(mainScreen);

        if (tally.currentScore >= threshold) {
            round++;
            if(round > 3) {
                round = 1;
                ante++;
            }

            window winPopup = window(4, 40, 8, 30, "");
            winPopup.print("YOU WIN (you scored over %d)\nPress any key to quit\n", threshold);
            wrefresh(winPopup.content);
            getchar();
            delwin(winPopup.content);
            delwin(winPopup.frame);
            break;
        }
        
        if (plays ==  0) {
            window lossPopup = window(4, 40, 8, 30, "");
            lossPopup.print("YOU LOSE (you ran out of hands)\nPress any key to quit\n");
            wrefresh(lossPopup.content);
            getchar();
            delwin(lossPopup.content);
            delwin(lossPopup.frame);
            break;
        }
    }
}

static void draw() {
    if(d.cards.empty()) return;
    h.add(d.cards.back());
    d.cards.pop_back();
}

static void playHand() {
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

static void discardHand() {
    if (discards == 0) return;
    recentScore = 0;
    if (h.cardsSelected() == 0) return;
    played = hand(h.popSelected());
    played.cursor = -1;
    for (int i = 0; i < played.selected.size(); i++) draw();
    played = hand(); // "play" an empty hand
    discards--;
}

static void updateInfo(window w) {
    werase(w.content);
    w.print("Small Blind\n");
    w.print("Threshold: %d\n", threshold);
    w.print("Score: %d\n", tally.currentScore);
    w.print("%s\n", handName(played.scoreType()));
    w.print("+%d\n", recentScore);
    w.print("Hands  Discards\n");
    w.print("  %d       %d\n", plays, discards);
    wrefresh(w.content);
}

static void updateScreen(window w) {
    werase(w.content);
    h.print(w.content);
    wmove(w.content, 3, 0);
    played.print(w.content);
    wrefresh(w.content);
}