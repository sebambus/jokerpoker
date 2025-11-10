#pragma once
#include <ncurses.h>

// initializes every combo of 3-bit colors as colorpair
void initcolors();
// sets foreground (fg) and background (bg) with ncurses constants
void setcolor(WINDOW* win, short fg, short bg);
void unsetcolor(WINDOW* win, short fg, short bg);