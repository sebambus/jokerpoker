#define _XOPEN_SOURCE_EXTENDED // needed to run unicode??
#include <ncursesw/ncurses.h>
#include <vector>
#include <locale>
#include "deck.h"
#include "card.h"
#include "color.h"
#include "hand.h"
#include "scorekeep.h"

deck d;
int playsLeft = 100;
int discardsLeft = 100;
scorekeep scoreKeep;

void playlevel() {
    level l;
    l.setupLevel();

    while(true) {
        clear();
        l.printLevel();
        refresh();

        switch (getchar()) {
            case 'q': // quit
                return;
            case 'h': // left (vim)
                l.h.moveCursor(-1);
                break;
            case 'l': // right (vim)
                l.h.moveCursor(1);
                break;
            case ' ': // select
                if (l.h.cardsSelected() < 5) //maximum playable hand
                    l.h.selected[l.h.cursor] = !l.h.selected[l.h.cursor];
                else   
                    l.h.selected[l.h.cursor] = false;
                break;
            case 'p': // play
                if (l.plays == 0) break;
                l.played = hand(l.h.popSelected());
                if (l.played.cards.size() == 0){l.recentScore = 0; break;} 
                l.played.cursor = -1;
                for (int i = 0; i < l.played.selected.size(); i++) { //draw new cards
                    l.h.add(l.d.cards.back()); //CAUSE OF SEG FAULT
                    l.d.cards.pop_back();
                }
                l.recentScore = l.tally.calculateScore(l.played); //add score of hand
                l.plays--;
                break;
            case 'd': // discard
                if (l.discards == 0) break;
                l.played = hand(l.h.popSelected());
                if (l.played.cards.size() == 0){l.recentScore = 0; break;}
                l.played.cursor = -1;
                for (int i = 0; i < l.played.selected.size(); i++) { //draw new cards
                    l.h.add(l.d.cards.back());
                    l.d.cards.pop_back();
                }
                l.played = hand(); // "play" an empty hand
                l.discards--;
                break;
        }
        
    }
}

int main() {
    setlocale(LC_ALL, "");  // switch to UTF-8??
    initscr();              // boot ncurses
    start_color();          // boot ncurses-color
    initcolors();           // initialize colors for color.h

    playlevel();

    endwin();               // close ncurses
    return 0;
}