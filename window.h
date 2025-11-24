#pragma once
#include <ncursesw/ncurses.h>
#include <string>

class level;
class game;
class shop;

class window {
public:
    WINDOW* frame;
    WINDOW* content;

    int width = 0;

    window(int h, int w, int y, int x, const char* title);
    void print(const char *format, ...);
    void printAndAutoColor(const char* str);
    void printWordInColor(const char* w, short fg, short bg);
    std::string textWrap(const char*);
    void updateLevelInfo(level* l);
    void updateGameInfo(game* g);
    void updateLevelScreen(level* l);
    void updateShopScreen(game* g, int index);
    void updateSpecialScreen(game* g, int index);
    void updateJokerScreen(game* g, int index);
    void updateCardInfo(game* g, int index, int s);
    void updateShopCardInfo(game* g, int index);
    void updatePeekScreen(level* l);
    void clear();
    void changeTitle(const char* title);
};