#include <stdio.h>
#include <stdlib.h>
#include <ncursesw/ncurses.h>
#include "game.h"
#include "voucher.h"

static voucher v;

static void updateScreen(window w);

void game::runshop(){
    v = (voucher) (rand() % VOUCHER_COUNT);

    mainScreen.changeTitle("Shop");
    updateScreen(mainScreen);
    getch();
    return;
}

static void updateScreen(window w) {
    werase(w.content);
    w.print("Vouchers:%s\n", voucherName(v));
    wrefresh(w.content);
}