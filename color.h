#pragma once
#include <ncurses.h>

// converts 3-bit color to ncurses constant
short decodecolor(int c);
// converts ncurses constant to 3-bit color
int encodecolor(short c);
// initializes every combo of 3-bit colors as colorpair
void initcolors();
// sets foreground (fg) and background (bg) with ncurses constants
void setcolor(WINDOW* win, short fg, short bg);
void unsetcolor(WINDOW* win, short fg, short bg);