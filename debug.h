#pragma once

#include <fstream>
#include <string>
#include <iostream>

inline void clearDebug(){
    FILE* txt = fopen("debug.txt", "w");
    if (txt == NULL) return;
    fclose(txt);
}

inline void debug(std::string message){
    FILE* txt = fopen("debug.txt", "a");
    if (txt == NULL) return;
    fprintf(txt, "%s\n", message.c_str());
    fclose(txt);
}