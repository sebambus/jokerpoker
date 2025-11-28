#pragma once
#include <ncursesw/ncurses.h>
#include <string>

class level;
class game;
class shop;

enum screentype : int{
    MAIN_SCREEN,
    CONSUMABLE_SCREEN,
    JOKER_SCREEN,
    LEVEL_INFO_SCREEN,
    GAME_INFO_SCREEN,
    CARD_INFO_SCREEN,
    SHOP_CARD_INFO_SCREEN,
    PEEK_SCREEN,
    SCREEN_TYPE_COUNT
};

class window {
public:
    WINDOW* frame;
    WINDOW* content;
    screentype type;
    game* g;

    int width = 0;

    window(int h, int w, int y, int x, const char* title, game* g, screentype type);
    void print(const char *format, ...);
    void printAndAutoColor(const char* str);
    void printWordInColor(const char* w, short fg, short bg);
    std::string textWrap(std::string s);
    void clear();
    void changeTitle(const char* title);

    void update(int, int);

    void updateLevelInfo();
    void updateGameInfo();
    void updateLevelScreen();
    void updateShopScreen(int index);
    void updateSpecialScreen(int index);
    void updateJokerScreen(int index);
    void updateCardInfo(int index, int s);
    void updateShopCardInfo(int index);
    void updatePeekScreen();
};