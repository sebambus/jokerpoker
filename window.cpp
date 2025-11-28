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
#include "debug.h"

window::window(int h, int w, int y, int x, const char *title, game *g, screentype type) {
    frame = newwin(h, w, y, x);
    content = newwin(h-2, w-2, y+1, x+1);
    width = w-2;
    refresh();
    changeTitle(title);
    this->g = g;
    this->type = type;
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
    
    // create vector of words
    std::string word = "";
    for (int i = 0; i < text.size(); i++) // check each letter
    {
        char c = text[i];
        if (c == ' ' || c == '\n'){ // if it's a space or newline, add it and the word to the vector
            word += c;
            words.push_back(word);
            word = "";
        } else if (c == '(' || c == ')' || c == ',') {
            if (word.size() != 0) words.push_back(word); // push back whatever was before it
            word = c;
            words.push_back(word); // add special character as it's own word, for proper coloring down the road
            word = "";
        } else { // normal character, append to word
            word += c;
        }
    }
    words.push_back(word);

    long unsigned int npos = std::string::npos;
    // print word one by one
    for (int i = 0; i < words.size(); i++)
    {
        std::string w = words[i];
        const char* cstr = w.c_str();
        if (w.find("Mult") != npos) // if word is "Mult"
            printWordInColor(cstr, COLOR_RED, COLOR_BLACK);
        else if (w.find("Chip") != npos) // if word is "Chips"
            printWordInColor(cstr, COLOR_BLUE, COLOR_BLACK);
        else if (w.find("$") != npos)
            printWordInColor(cstr, COLOR_YELLOW, COLOR_BLACK);
        else if (w.find("Spade") != npos)
            printWordInColor(cstr, COLOR_BLACK, COLOR_WHITE);
        else if (w.find("Heart") != npos)
            printWordInColor(cstr, COLOR_BLACK, COLOR_RED);
        else if (w.find("Club") != npos)
            printWordInColor(cstr, COLOR_BLACK, COLOR_BLUE);
        else if (w.find("Diamond") != npos)
            printWordInColor(cstr, COLOR_WHITE, COLOR_YELLOW);
        else if (w.find("Tarot") != npos || w.find("Planet") != npos)
            printWordInColor(cstr, COLOR_MAGENTA, COLOR_BLACK);
        else if (w.find("+") != npos || w.find("X") != npos || w.find("-") != npos){ // if word has a plus or X, check to see whats after
            if (words[i + 1].find("Mult") != npos)
                printWordInColor(cstr, COLOR_RED, COLOR_BLACK);
            else if (words[i + 1].find("Chip") != npos){
                printWordInColor(cstr, COLOR_BLUE, COLOR_BLACK);
            }
            else 
                print(cstr);  
        }
        else 
            print(cstr);
    }

}

void window::printWordInColor(const char* w, short fg, short bg){
    char* word = (char*)w;
    setcolor(content, fg, bg);
    bool hasSpace = false;
    int wSize = 0;
    for (int i = 0; word[i] != '\0'; i++){
        if (word[i] == ' ')
            hasSpace = true;
        wSize++;
    }
    
    // if the word has a space, print it without the space, then unset color and print the space
    // this is so highlighting words looks correct, only the word and no spaces are highlighted
    if (hasSpace){
        word[wSize-1] = '\0';
        print(word);
        unsetcolor(content, fg, bg);
        print(" ");    
    } else{
        print(word);
        unsetcolor(content, fg, bg);
    }
}

std::string window::textWrap(std::string s){
    std::string newstr = s;
    std::vector<std::string> lines;

    while (newstr.size() > width){
        int i = width - 1; // start looking at 1 less than the width
        while (newstr[i] != ' '){ // move backwards until you find a space
            i--;
        }
        lines.push_back(newstr.substr(0, i)); // once you find one, save everything before that space
        newstr.erase(0,i+1); // clear that area from the string, repeat the loop
    }
    lines.push_back(newstr); // add remainder

    std::string final = "";
    for (int i = 0; i < lines.size(); i++) // add all the lines back to a single string, now with newlines
    {
        final += lines[i] + "\n";
    }
    
    return final;
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

void window::update(int index, int s) {
    switch(type) {
    case MAIN_SCREEN:
        if(g->phase == LEVEL_PHASE)
            updateLevelScreen();
        if(g->phase == SHOP_PHASE)
            updateShopScreen(index);
    case CONSUMABLE_SCREEN:
        updateSpecialScreen(index);
    case JOKER_SCREEN:
        updateJokerScreen(index);
    case LEVEL_INFO_SCREEN:
        updateLevelInfo();
    case GAME_INFO_SCREEN:
        updateGameInfo();
    case CARD_INFO_SCREEN:
        updateCardInfo(index, s);
    case SHOP_CARD_INFO_SCREEN:
        updateShopCardInfo(index);
    case PEEK_SCREEN:
        updatePeekScreen();
    }
}

void window::updateLevelInfo() {
    werase(content);
    print("Small Blind\n");
    print("Threshold: %d\n", g->l->threshold);
    print("Score: %d\n", g->l->currentScore);
    if (g->l->played.pokerHand == HAND_TYPE_COUNT) // for when level starts
        print("\n\n\n");
    else{
        print("%s\n", handName(g->l->played.pokerHand));
        setcolor(content, COLOR_BLACK, COLOR_RED);
        print("Mult: %d ",g->l->recentMult);
        setcolor(content, COLOR_BLACK, COLOR_BLUE);
        print("Chips: %d\n", g->l->recentChips);
        unsetcolor(content, COLOR_BLACK, COLOR_BLUE);
        print("+%d\n", g->l->recentScore);
    }
    print("Hands  Discards\n");
    print("  %d       %d\n", g->l->plays, g->l->discards);
    wrefresh(content);
}

void window::updateGameInfo() {
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

void window::updateLevelScreen() {
    werase(content);
    g->l->h.print(content);
    wmove(content, 5, 0);
    g->l->played.print(content);
    wrefresh(content);
}

void window::updateShopScreen(int index) {
    if (g->phase == LEVEL_PHASE) // dont do anything if your not in the shop
        return;

    werase(content);

    std::vector<shopItem> items;
    if (g->s->mode == DEFAULT_MODE){
        items = g->s->shopItems;
        changeTitle("Shop");
    }
    else items = g->s->packItems;

    std::string header;
    for (int i = 0; i < items.size(); i++){
        shopItem si = items[i];
        std::string newHeader;
        if (si.typeOfItem == SI_CARD || si.typeOfItem == SI_ITEM)
            newHeader = "Items:\n";
        else if (si.typeOfItem == SI_PACK)
            newHeader = "Packs:\n";
        else if (si.typeOfItem == SI_VOUCHER)
            newHeader = "Voucher:\n";
        
        if (newHeader != header){
            print("%s", newHeader.c_str());
            header = newHeader;
        }


        if (i == index)
            print("[x] ");
        else
            print("[ ] ");
        print("%s ", si.getName().c_str());
        if (g->s->mode == DEFAULT_MODE)
            printWordInColor(("$"+std::to_string(si.cost)).c_str(), COLOR_YELLOW, COLOR_BLACK);
        print("\n");
    }

    if (g->s->mode == PACK_MODE)
        g->s->modifyableCards.print(content);
    
    if (index != -1) g->cardInfo.updateShopCardInfo(index);

    wrefresh(content);
}

void window::updateSpecialScreen(int index){
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
    if (index != -1) g->cardInfo.updateCardInfo(index, static_cast<int>(CONSUMABLE_SCREEN));
    wrefresh(content);
}

void window::updateJokerScreen(int index){
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

    if (index != -1) g->cardInfo.updateCardInfo(index, static_cast<int>(JOKER_SCREEN));
    wrefresh(content);
}

// prints info on item based on game's focusScreen and the items position in it's respective vector
// s is screen. when calling this, cast a screentype to an int
void window::updateCardInfo(int index, int s){
    if (s == static_cast<int>(CONSUMABLE_SCREEN)) // don't display anything if screen is empty
        if (g->consumables.size() == 0)
            return;

    if (s == static_cast<int>(JOKER_SCREEN))
        if (g->jokers.size() == 0)
            return;

    werase(content);
    std::string desc;
    if (s == static_cast<int>(CONSUMABLE_SCREEN))
        desc = g->consumables[index].description();
    else
        desc = g->jokers[index].description();

    printAndAutoColor(textWrap(desc).c_str()); // wrap text and convert back to char* before printing;
    
    wrefresh(content);
}

// specifically for printing out the description of the selected item in shop
// needs to be it's own function for how much differently it works than updateCardInfo
void window::updateShopCardInfo(int index){
    werase(content);
    shopItem si;
    if (g->s->mode == DEFAULT_MODE)
        si = g->s->shopItems[index];
    else 
        si = g->s->packItems[index];

    std::string desc;

    if (si.typeOfItem == SI_ITEM){
        desc = si.i.description();
    } else if (si.typeOfItem == SI_PACK){
        desc = description(si.p).c_str();
    } else if (si.typeOfItem == SI_VOUCHER){
        desc = item(si.v).description();
    } else if (si.typeOfItem == SI_CARD){
        desc = si.c.name();
    }

    printAndAutoColor(textWrap(desc).c_str());

    wrefresh(content);
}


void window::updatePeekScreen(){
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
            print(" %d |", g->l->d.specificCount(s,j)); // print cells
        }
        print(" %d", g->l->d.suitCount(s)); // print suit row total
        print("\n");

        unsetcolor(content, suitColor, COLOR_BLACK);
    }

    print("   ");
    for (int i = 1; i < 14; i++){
        print("  %d ", g->l->d.cardCount(i)); // print value collumn total
    }

    wrefresh(content);
}
