#include "shop.h"
#include <stdio.h>
#include <ncursesw/ncurses.h>

shop::shop(){

}

void shop::enter(){
    printw("Entered the shop");
    getch();
    return;
}