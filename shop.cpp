#include "shop.h"
#include <stdio.h>
#include <ncursesw/ncurses.h>

shop::shop(game *game){
    g = game;
    run();
}

void shop::run(){
    printw("Entered the shop");
    getch();
    return;
}