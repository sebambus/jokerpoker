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

void window::printAndAutoColor(const char* str){
    std::string text(str);
    std::vector<std::string> words;
    std::vector<int> spaces; 

    spaces.push_back(-1);
    for (int i = 0; i < text.size(); i++){ // find position of all spaces
        if (text[i] == ' ')
            spaces.push_back(i);
    }
    spaces.push_back(text.size() + 1);

    for (int i = 0; i < spaces.size() - 1; i++) // split text by spaces
    {
        std::string word = text.substr(spaces[i] + 1, spaces[i+1] - spaces[i]);
        words.push_back(word);
    }
    
    // if there are any 'words' with a newline, split them
    // a jank workaround, but it works for now
    std::vector<std::string> copy;
    copy = words;
    words.clear();

    for (std::string w : copy){
        int pos = w.find('\n');
        if (pos == std::string::npos)
            words.push_back(w);
        else {
            std::string s1 = w.substr(0,pos);
            std::string s2 = w.substr(pos, w.size() - pos);
            words.push_back(s1);
            words.push_back(s2);
        }
    }

    // print word one by one
    for (int i = 0; i < words.size(); i++)
    {
        std::string w = words[i];
        const char* cstr = w.c_str();
        if (w.find("Mult") != std::string::npos) // if word is "Mult"
            printWordInColor(cstr, COLOR_RED, COLOR_BLACK);
        else if (w.find("Chip") != std::string::npos) // if word is "Chips"
            printWordInColor(cstr, COLOR_BLUE, COLOR_BLACK);
        else if (w.find("$") != std::string::npos)
            printWordInColor(cstr, COLOR_YELLOW, COLOR_BLACK);
        else if (w.find("Spade") != std::string::npos)
            printWordInColor(cstr, COLOR_WHITE, COLOR_BLACK);
        else if (w.find("Heart") != std::string::npos)
            printWordInColor(cstr, COLOR_RED, COLOR_BLACK);
        else if (w.find("Club") != std::string::npos)
            printWordInColor(cstr, COLOR_BLUE, COLOR_BLACK);
        else if (w.find("Diamond") != std::string::npos)
            printWordInColor(cstr, COLOR_YELLOW, COLOR_BLACK);
        else if (w.find("+") != std::string::npos){ // if word has a plus, check to see whats after
            if (words[i + 1].find("Mult") != std::string::npos)
                printWordInColor(cstr, COLOR_RED, COLOR_BLACK);
            else if (words[i + 1].find("Chip") != std::string::npos){
                printWordInColor(cstr, COLOR_BLUE, COLOR_BLACK);
            }
        }
        else 
            print(cstr);
    }

}

void window::printWordInColor(const char* w, short fg, short bg){
    setcolor(content, fg, bg);
    print(w);
    unsetcolor(content, fg, bg);
}


std::string window::textWrap(const char* cstring){
    std::string newstr(cstring);
    std::vector<std::string> lines;

    while (newstr != ""){
        int i = width - 1; // start looking at 1 less than the width
        while (newstr[i] != ' '){ // move backwards until you find a space
            i--;
        }
        lines.push_back(newstr.substr(0, i)); // once you find one, save everything before that space
        newstr.erase(0,i+1); // clear that area from the string, repeat the loop
    }

    std::string final = "";
    for (int i = 0; i < lines.size(); i++) // add all the lines back to a single string, now with newlines
    {
        final += lines[i] + "\n";
    }
    
    return final;
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
        print("%c - $%d %s\n", menuchar++, i.cost, i.name());
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
    if (index != -1) g->cardInfo.updateCardInfo(g, index, static_cast<int>(CONSUMABLE_SCREEN));
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

    if (index != -1) g->cardInfo.updateCardInfo(g, index, static_cast<int>(JOKER_SCREEN));
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

    printAndAutoColor(textWrap(desc).c_str()); // wrap text and convert back to char* before printing;
    
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
