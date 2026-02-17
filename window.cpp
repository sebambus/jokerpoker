#include "window.h"

#include <cstdarg>
#include <string>
#include <sstream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <string.h>
#include "level.h"
#include "game.h"
#include "shop.h"
#include "item.h"
#include "card.h"
#include "color.h"
#include "debug.h"
#include "readcsv.h"

window::window(int h, int w, int y, int x, const char *title, game *g, screentype type) {
    frame = newwin(h, w, y, x);
    content = newwin(h-2, w-2, y+1, x+1);
    width = w-2;
    refresh();
    changeTitle(title);
    this->g = g;
    this->type = type;
}

// find and replace char function
const char* replaceChar(const char* s, char oldchar, char newchar) {
    std::string str(s);
    for(size_t i = 0; i < str.size(); i++)
        if(str[i] == oldchar)
            str[i] = newchar;
    char* result = new char[str.size() + 1];
    std::strcpy(result, str.c_str());
    return result;
}

void window::print(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vw_printw(content, replaceChar(format, '{', ','), args);
    va_end(args);
}

// print string out and automatically color the correct words
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

// helper function used by printAndAutoColor
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

// returns a string with newlines that wraps the text based on this windows size
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
    wrefresh(content);
}

void window::changeTitle(const char *title) {
    werase(frame);
    box(frame, 0, 0);
    wprintw(frame, "%s", title);
    wrefresh(frame);
}

// universal update function
void window::update(int index) {
    werase(content);
    switch(type) {
    case MAIN_SCREEN:
        if(g->phase == LEVEL_PHASE)
            updateLevelScreen();
        if(g->phase == SHOP_PHASE)
            updateShopScreen(index);
	break;
    case CONSUMABLE_SCREEN:
        updateSpecialScreen(index);
	break;
    case JOKER_SCREEN:
        updateJokerScreen(index);
	break;
    case LEVEL_INFO_SCREEN:
        updateLevelInfo();
	break;
    case GAME_INFO_SCREEN:
        updateGameInfo();
	break;
    case CARD_INFO_SCREEN:
        if (g->focusScreen == MAIN_SCREEN)
            updateShopCardInfo(index);
        else
            updateCardInfo(index);
	break;
    case PEEK_SCREEN:
        updatePeekScreen();
	break;
    case PLAYING_CARD_INFO_SCREEN:
        updatePlayingCardInfo(index);
    break;
    }
    wrefresh(content);
}

void window::updateLevelInfo() {
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
}

void window::updateGameInfo() {
    print("You start with\n");
    print("Hands Discards\n");
    print("  %d      %d\n", g->getPlays(), g->getDiscards());
    print("Money: ");
    setcolor(content, COLOR_YELLOW, COLOR_BLACK);
    print("$%d\n", g->money);
    unsetcolor(content, COLOR_YELLOW, COLOR_BLACK);
    print("Ante %d/%d, Round %d\n", g->ante, 8, g->round);
}

void window::updateLevelScreen() {
    g->l->h.print(content);
    wmove(content, 5, 0);
    g->l->played.print(content);
}

void window::updateShopScreen(int index) {
    if (g->phase == LEVEL_PHASE) // dont do anything if your not in the shop
        return;

    // if we are now in pack mode
    if (g->s->mode == PACK_MODE){
        int shopIndex = 0;
        for (item i : g->s->packItems)
        {
            if (shopIndex == index)
                print("[x] ");
            else
                print("[ ] ");
            print("%s\n", i.name());
            shopIndex++;
        }
        g->s->modifyableCards.print(content);
        if (index != -1) g->cardInfo.update(index);
        return;
    }

    // if we are in default mode
    int shopIndex = 0;
    if (g->s->items.size() != 0) print("Items:\n");
    for (item i : g->s->items)
    {
        if (shopIndex == index)
            print("[x] ");
        else
            print("[ ] ");
        print("%s ", i.name());
        std::string costString = "$" + std::to_string(i.cost);
        printWordInColor(costString.c_str(), COLOR_YELLOW, COLOR_BLACK);
        print("\n");
        shopIndex++;
    }
    
    if (g->s->packs.size() != 0) print("Packs:\n");
    for (pack p : g->s->packs){
        if (shopIndex == index)
            print("[x] ");
        else
            print("[ ] ");
        print("%s ", name(p).c_str());
        std::string costString = "$" + std::to_string(p.cost);
        printWordInColor(costString.c_str(), COLOR_YELLOW, COLOR_BLACK);
        print("\n");
        shopIndex++;
    }

    // if shop has a voucher
    if (g->s->v != VOUCHER_COUNT){
        print("Voucher:\n");
        if (index == g->s->packs.size() + g->s->items.size())
            print("[x] ");
        else    
            print("[ ] ");
        item vItem = item(g->s->v);
        const char* str = vItem.name();
        print("%s ", str);
        std::string costString = "$" + std::to_string(vItem.cost);
        printWordInColor(costString.c_str(), COLOR_YELLOW, COLOR_BLACK);
    }

    if (index != -1) g->cardInfo.update(index);
}

// update consumable screen
void window::updateSpecialScreen(int index){
    if (g->consumables.size() == 0) //for when you start a level without any consumables
        return;

    for (int i = 0; i < g->consumables.size(); i++){
        if (i == index)
            print("[x]");
        else
            print("[ ]");
        print("%s\n", g->consumables[i].name());
    }
    if (index != -1) g->cardInfo.update(index);
}

void window::updateJokerScreen(int index){
    if (g->jokers.size() == 0) //for when you start a level without any consumables
        return;

    for (int i = 0; i < g->jokers.size(); i++){
        if (i == index)
            print("[x]");
        else
            print("[ ]");
        print("%s\n", g->jokers[i].name());
    }

    if (index != -1) g->cardInfo.update(index);
}

// prints info on item based on game's focusScreen and the items position in it's respective vector
void window::updateCardInfo(int index){
    if (g->focusScreen == CONSUMABLE_SCREEN) // don't display anything if screen is empty
        if (g->consumables.size() == 0)
            return;

    if (g->focusScreen == JOKER_SCREEN)
        if (g->jokers.size() == 0)
            return;

    std::string desc;
    if (g->focusScreen == CONSUMABLE_SCREEN)
        desc = g->consumables[index].description();
    else
        desc = g->jokers[index].description();

    printAndAutoColor(textWrap(desc).c_str()); // wrap text and convert back to char* before printing;
    
}

// specifically for printing out the description of the selected item in shop
// needs to be it's own function for how much differently it works than updateCardInfo
void window::updateShopCardInfo(int index){
    std::string desc;

    // if we are in pack mode
    if (g->s->mode == PACK_MODE){
        desc = g->s->packItems[index].description();
    }

    // if we are looking at an item
    else if (index >=0 && index < g->s->items.size()){
        desc = g->s->items[index].description();
    }
    // if we are looking at a pack
    else if (index >= g->s->items.size() && index < g->s->items.size() + g->s->packs.size()){
        desc = description(g->s->packs[index - g->s->items.size()]);
    }
    // if we are looking at a voucher
    else{
        item voucherItem = item(g->s->v);
        desc = voucherItem.description();
    }

    printAndAutoColor(textWrap(desc).c_str());
}

// update window below playing area that describes the playing cards
void window::updatePlayingCardInfo(int index){
    if (index < 0) return;

    card c;
    if (g->phase == SHOP_PHASE && g->s->modifyableCards.cards.size() != 0)
        c = g->s->modifyableCards.cards[index];
    else if (g->phase == LEVEL_PHASE)
        c = g->l->h.cards[index];
    else 
        return;

    printAndAutoColor(textWrap(c.name()).c_str());

}

void window::updatePeekScreen(){
    // printing out deck
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
        print("  %d ", g->l->d.cardValueCount(i)); // print value collumn total
    }

    // printing out hands
    print("\n\n");
    print("                 | Mult  | Chips | Level | Times Played\n");
    for (int i = 0; i < 13; i++)
    {
        const char* handName = readcsv("handtable.csv", i, 0);
        for (int j = 0; j < 15 - (int)strlen(handName); j++)
        {
            print(" ");
        }
        
        print("%s: |", handName);

        for (int x = 0; x < 4; x++)
        {
            std::string value = std::to_string(g->handTable[i][x]);
            int valSize = value.size();
            int leftPad = 0; int rightPad = 0;
            int combined = valSize + leftPad + rightPad;
            while (true){
                if (combined >= 7)
                    break;
                leftPad++;
                combined++;
                if (combined >= 7)
                    break;
                rightPad++;
                combined++;
            }

            for (int s = 0; s < leftPad; s++){
                print(" ");
            }
            if (x == 0) setcolor(content, COLOR_RED, COLOR_BLACK);
            if (x == 1) setcolor(content, COLOR_BLUE, COLOR_BLACK);
            print(value.c_str());
            setcolor(content, COLOR_WHITE, COLOR_BLACK);
            for (int s = 0; s < rightPad; s++){
                print(" ");
            }
            if (x != 3) print("|");
        }
        print("\n");
    }
}