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
        if(g->phase == BLINDS_PHASE)
            updateBlindsScreen();
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
    print(g->l->b.name.c_str()); // name of current blind
    print("\nThreshold: %d\n", g->l->threshold);
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
            // auto color only card names
            if (i.type == CARD){
                printAndAutoColor(i.name().c_str());
                print("\n");
            }
            else print("%s\n", i.name().c_str());
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
        if (i.type == CARD){
            printAndAutoColor(i.name().c_str());
            print("\n");
        }
        else print("%s ", i.name().c_str());
        
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
        const char* str = vItem.name().c_str();
        print("%s ", str);
        std::string costString = "$" + std::to_string(vItem.cost);
        printWordInColor(costString.c_str(), COLOR_YELLOW, COLOR_BLACK);
    }

    if (index != -1) g->cardInfo.update(index);
}

void window::updateBlindsScreen(){
    for (int i = 0; i < 3; i++)
    {
        std::string nextString = "Next -> ";
        if (g->round == i +1) printWordInColor(nextString.c_str(), COLOR_BLACK, COLOR_WHITE);
        print("%s", g->currBlinds[i].name.c_str());
        print(" | Score: ");
        std::string scoreString = std::to_string((int)(g->antebases[g->ante] * g->currBlinds[i].thresholdMultiplier));
        printWordInColor(scoreString.c_str(), COLOR_BLUE, COLOR_BLACK);
        print(" | Reward: ");
        std::string rewardString = "$" + std::to_string(g->currBlinds[i].reward);
        printWordInColor(rewardString.c_str(), COLOR_YELLOW, COLOR_BLACK);
        if (i == 2) print("\n%s", g->currBlinds[i].description.c_str());
        else {
            tag t = g->currBlinds[i].skipTag;
            std::string skipText = "\nSkip: " +  t.name + ", " + t.description;
            print(skipText.c_str());
        }
        print("\n\n");
    }
    
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
        print("%s\n", g->consumables[i].name().c_str());
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
        print("%s\n", g->jokers[i].name().c_str());
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