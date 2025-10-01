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
int playsLeft = 4;
int discardsLeft = 3;
scorekeep scoreKeep;

void playlevel() {
    d.fillDeck();
    d.shuffle();

    hand h;
    // deals 8 cards
    for (int i = 0; i < 8; i++) {
        h.add(d.cards.back());
        d.cards.pop_back();
    }

    hand played;
    int addedScore = 0;    
    while(true) {
        // update screen
        clear();
        printw("Score: %d\n", scoreKeep.currentScore);
        printw("Plays: %d\n", playsLeft); printw("Discards: %d\n", discardsLeft);
        h.print();
        handtype scoredHandType = played.scoreType();

        if (addedScore > 0){
            printw("%s\n", handName(scoredHandType));
            printw("+%d Score\n", addedScore);
        }
        
        played.print();
        refresh();

        // read keyboard input with switch
        switch (getchar()) {
            // q - quit
            case 'q':
                return;
            // h - left (vim)
            case 'h':
                h.moveCursor(-1);
                break;
            // l - right
            case 'l':
                h.moveCursor(1);
                break;
            // _ - select
            case ' ':
                if (h.cardsSelected() < 5) //maximum playable hand
                    h.selected[h.cursor] = !h.selected[h.cursor];
                else   
                    h.selected[h.cursor] = false;
                break;
            // p - play
            case 'p':
                if (playsLeft == 0) break;
                played = hand(h.popSelected());
                if (played.cards.size() == 0){addedScore = 0; break;} 
                played.cursor = -1;
                for (int i = 0; i < played.selected.size(); i++) { //draw new cards
                    h.add(d.cards.back());
                    d.cards.pop_back();
                }
                addedScore = scoreKeep.calculateScore(played); //add score of hand
                playsLeft--;
                break;
            // d - discard
            case 'd':
                if (discardsLeft == 0) break;
                played = hand(h.popSelected());
                if (played.cards.size() == 0){addedScore = 0; break;}
                played.cursor = -1;
                for (int i = 0; i < played.selected.size(); i++) { //draw new cards
                    h.add(d.cards.back());
                    d.cards.pop_back();
                }
                played = hand(); // "play" an empty hand
                addedScore = 0;
                discardsLeft--;
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