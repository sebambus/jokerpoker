#include "window.h"

#include <cstdarg>
#include <string>
#include <sstream>
#include "level.h"
#include "game.h"
#include "shop.h"
#include "item.h"
#include "card.h"
#include "color.h"

window::window(int h, int w, int y, int x, const char* title) {
    frame = newwin(h, w, y, x);
    content = newwin(h-2, w-2, y+1, x+1);
    width = w-2;
    refresh();
    changeTitle(title);
}

void window::print(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vw_printw(content, format, args);
    va_end(args);
}

std::string window::textWrap(const char* cstring){
    std::string str(cstring);
    std::stringstream ss(str);
    std::string word;
    std::vector<std::string> words;
    std::string newstr = "";

    while (ss >> word){ // create vector of individual words in string
        words.push_back(word);
    }

    int lineSize = 0; // keep track of characters in current line
    for (std::string w : words){
        w += " "; 
        // if adding the next word would exceed the size of the window
        if (lineSize + w.size() >= width){
            newstr += "\n" + w; // add a newline before the word
            lineSize = w.size() - 1; // now the size of the new line is the size of that word minus \n
        }
        else { // add word to string as normal
            newstr += w;
            lineSize += w.size();
        }      
    }

    return newstr;
}

void window::updateLevelInfo(level* l) {
    werase(content);
    print("Small Blind\n");
    print("Threshold: %d\n", l->threshold);
    print("Score: %d\n", l->currentScore);
    print("%s\n", handName(l->played.scoreType()));
    print("+%d\n", l->recentScore);
    print("Hands  Discards\n");
    print("  %d       %d\n", l->plays, l->discards);
    wrefresh(content);
}

void window::updateGameInfo(game *g) {
    werase(content);
    print("You start with\n");
    print("Hands Discards\n");
    print("  %d      %d\n", g->getPlays(), g->getDiscards());
    print("Money: ");
    setcolor(content, COLOR_YELLOW, COLOR_BLACK);
    print("$%d\n", g->money);
    unsetcolor(content, COLOR_YELLOW, COLOR_BLACK);
    print("Ante %d/%d, Round %d\n", g->ante, 8, g->round);
    wrefresh(content);
}

void window::updateLevelScreen(level *l) {
    werase(content);
    l->h.print(content);
    wmove(content, 3, 0);
    l->played.print(content);
    wrefresh(content);
}

void window::updateShopScreen(shop *s) {
    werase(content);
    char menuchar =  'a';
    if(s->items.size() > 0) {
        print("Items:\n");
    }
    for(item i : s->items) {
        print("%c - $%d %s\n", menuchar++, 3, i.name());
    }
    if(s->v != VOUCHER_COUNT)
        print("Vouchers:\n%c - $%d %s\n", menuchar++, 10, item(s->v).name());
    wrefresh(content);
}

void window::updateSpecialScreen(game* g, int index){
    if (g->consumables.size() == 0) //for when you start a level without any consumables
        return;

    werase(content);
    for (int i = 0; i < g->consumables.size(); i++){
        if (i == index)
            print("[x]");
        else
            print("[ ]");
        print("%s\n", g->consumables[i].name());
    }
    g->cardInfo.updateCardInfo(g, index, static_cast<int>(CONSUMABLE_SCREEN));
    wrefresh(content);
}

void window::updateJokerScreen(game* g, int index){
    werase(content);
    if (g->jokers.size() == 0) //for when you start a level without any consumables
        return;

    werase(content);
    for (int i = 0; i < g->jokers.size(); i++){
        if (i == index)
            print("[x]");
        else
            print("[ ]");
        print("%s\n", g->jokers[i].name());
    }
    g->cardInfo.updateCardInfo(g, index, static_cast<int>(JOKER_SCREEN));
    wrefresh(content);
}

// prints info on item based on game's focusScreen and the items position in it's respective vector
// s is screen. when calling this, cast a selectableScreen to an int
void window::updateCardInfo(game* g, int index, int s){
    if (g->consumables.size() == 0)
        return;

    werase(content);
    const char* desc;
    if (s == static_cast<int>(CONSUMABLE_SCREEN))
        desc = g->consumables[index].description();
    else
        desc = g->jokers[index].description();

    print(textWrap(desc).c_str()); // wrap text and convert back to char* before printing
    wrefresh(content);
}

void window::updatePeekScreen(level* l){
    werase(content);
    print("   ");
    for (int i = 1; i < 14; i++){ // 
        print("  %c ", valueToChar(i)); // print collumn headers
    }
    print("\n");


    for (int i = 0; i < 4; i++){
        suit s = static_cast<suit>(i);
        int suitColor = suitToColor(s);
        setcolor(content, suitColor, COLOR_BLACK); // color line with suit color

        print("%c: |", suitToChar(s)); // print row headers

        for (int j = 1; j < 14; j++){
            print(" %d |", l->d.specificCount(s,j)); // print cells
        }
        print(" %d", l->d.suitCount(s)); // print suit row total
        print("\n");

        unsetcolor(content, suitColor, COLOR_BLACK);
    }

    print("   ");
    for (int i = 1; i < 14; i++){
        print("  %d ", l->d.cardCount(i)); // print value collumn total
    }

    wrefresh(content);
}

void window::clear(){
    wclear(content);
    wclear(frame);
    wrefresh(content);
    wrefresh(frame);
}

void window::changeTitle(const char *title) {
    werase(frame);
    box(frame, 0, 0);
    wprintw(frame, "%s", title);
    wrefresh(frame);
}
