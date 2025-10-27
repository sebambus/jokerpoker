#include "window.h"

#include <cstdarg>
#include <string>
#include <sstream>
#include "level.h"
#include "game.h"
#include "shop.h"
#include "item.h"

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

    while (ss >> word){
        words.push_back(word);
    }

    int lineSize = 0;
    for (std::string w : words){
        w += " ";
        if (lineSize + w.size() >= width){
            newstr += "\n" + w;
            lineSize = w.size() - 1; // -1 becuse newline character is a character
        }
        else {
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
    print("Money: $%d\n", g->money);
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
        print("%s\n", g->consumables[i].name()); //must convert std::string to const char * for print function
    }
    g->cardInfo.updateCardInfo(g, index);
    wrefresh(content);
}

void window::updateCardInfo(game* g, int index){
    if (g->consumables.size() == 0)
        return;

    werase(content);
    print(textWrap(g->consumables[index].description()).c_str());
    wrefresh(content);
}

void window::updatePeekScreen(level* l){
    werase(content);
    print("   ");
    for (int i = 1; i < 14; i++){
        char c;
        switch (i){
        case 1:
            c = 'A';
            break;
        case 10:
            c = 'T';
            break;
        case 11:
            c = 'J';
            break;
        case 12:
            c = 'Q';
            break;
        case 13:
            c = 'K';
            break;
        default:
            c = i + '0';
            break;
        }
        print("  %c ", c);
    }
    print("\n");


    for (int i = 0; i < 4; i++){
        suit s = static_cast<suit>(i);

        char c;
        if (s == SPADE) c = 'S';
        else if (s == CLUB) c = 'C';
        else if (s == DIAMOND) c = 'D';
        else if (s == HEART) c = 'H';

        print("%c: |", c);

        for (int j = 1; j < 14; j++){
            print(" %d |", l->d.specificCount(s,j));
        }
        print(" %d", l->d.suitCount(s));
        print("\n");
    }

    print("   ");
    for (int i = 1; i < 14; i++){
        print("  %d ", l->d.cardCount(i));
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
