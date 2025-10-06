#include "window.h"

#include <cstdarg>

window::window(int h, int w, int y, int x, char* title) {
    frame = newwin(h, w, y, x);
    content = newwin(h-2, w-2, y+1, x+1);
    box(frame, 0, 0);
    wprintw(frame, "%s", title);
    refresh();
}

void window::print(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vw_printw(content, format, args);
    va_end(args);
}