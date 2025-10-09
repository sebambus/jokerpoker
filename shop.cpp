#include "shop.h"
#include <stdio.h>
#include <ncursesw/ncurses.h>

shop::shop(game *game){
    g = game;
    enter();
}

void shop::enter(){
    printw("Entered the shop");
    getch();
    return;
}