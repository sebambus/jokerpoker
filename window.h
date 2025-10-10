#pragma once
#include <ncursesw/ncurses.h>

class level;
class game;

class window {
public:
    WINDOW* frame;
    WINDOW* content;

    window(int h, int w, int y, int x, const char* title);
    void print(const char *format, ...);
    void update(level* l);
    void update(game* g);
    void clear();
};