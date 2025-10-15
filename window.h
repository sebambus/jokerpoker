#pragma once
#include <ncursesw/ncurses.h>

class level;
class game;
class shop;

class window {
public:
    WINDOW* frame;
    WINDOW* content;

    window(int h, int w, int y, int x, const char* title);
    void print(const char *format, ...);
    void updateShopScreen(shop* s);
    void clear();
    void changeTitle(const char* title);
};