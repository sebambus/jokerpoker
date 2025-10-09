#pragma once

#include <ncursesw/ncurses.h>
#include "level.h"

class window {
public:
    WINDOW* frame;
    WINDOW* content;

    window(int h, int w, int y, int x, char* title);
    void print(const char *format, ...);
    void update(level* l);
    void update(game* g);
    void clear();
};