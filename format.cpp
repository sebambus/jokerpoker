#include "window.h"
#include "color.h"
#include <vector>
#include <string>
#include <cstring>


// find and replace char function
const char* replaceChar(const char* s, char oldchar, char newchar) {
    std::string str(s);
    for(size_t i = 0; i < str.size(); i++)
        if(str[i] == oldchar)
            str[i] = newchar;
    char* result = new char[str.size() + 1];
    std::strcpy(result, str.c_str());
    return result;
}

void window::print(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vw_printw(content, replaceChar(format, '{', ','), args);
    va_end(args);
}

// print string out and automatically color the correct words
void window::printAndAutoColor(const char* str){
    std::string text(str);
    std::vector<std::string> words;
    
    // create vector of words
    std::string word = "";
    for (int i = 0; i < text.size(); i++) // check each letter
    {
        char c = text[i];
        if (c == ' ' || c == '\n'){ // if it's a space or newline, add it and the word to the vector
            word += c;
            words.push_back(word);
            word = "";
        } else if (c == '(' || c == ')' || c == ',') {
            if (word.size() != 0) words.push_back(word); // push back whatever was before it
            word = c;
            words.push_back(word); // add special character as it's own word, for proper coloring down the road
            word = "";
        } else { // normal character, append to word
            word += c;
        }
    }
    words.push_back(word);

    long unsigned int npos = std::string::npos;
    // print word one by one
    for (int i = 0; i < words.size(); i++)
    {
        std::string w = words[i];
        const char* cstr = w.c_str();
        if (w.find("Mult") != npos || w.find("Red") != npos) // if word is "Mult"
            printWordInColor(cstr, COLOR_RED, COLOR_BLACK);
        else if (w.find("Chip") != npos || w.find("Blue") != npos) // if word is "Chips"
            printWordInColor(cstr, COLOR_BLUE, COLOR_BLACK);
        else if (w.find("$") != npos || w.find("Gold") != npos)
            printWordInColor(cstr, COLOR_YELLOW, COLOR_BLACK);
        else if (w.find("Spade") != npos)
            printWordInColor(cstr, COLOR_BLACK, COLOR_WHITE);
        else if (w.find("Heart") != npos)
            printWordInColor(cstr, COLOR_BLACK, COLOR_RED);
        else if (w.find("Club") != npos)
            printWordInColor(cstr, COLOR_BLACK, COLOR_BLUE);
        else if (w.find("Diamond") != npos)
            printWordInColor(cstr, COLOR_WHITE, COLOR_YELLOW);
        else if (w.find("Tarot") != npos || w.find("Planet") != npos || w.find("Purple") != npos)
            printWordInColor(cstr, COLOR_MAGENTA, COLOR_BLACK);
        else if (w.find("+") != npos || w.find("X") != npos || w.find("-") != npos){ // if word has a plus or X, check to see whats after
            if (words[i + 1].find("Mult") != npos)
                printWordInColor(cstr, COLOR_RED, COLOR_BLACK);
            else if (words[i + 1].find("Chip") != npos){
                printWordInColor(cstr, COLOR_BLUE, COLOR_BLACK);
            }
            else 
                print(cstr);  
        }
        else 
            print(cstr);
    }

}

// helper function used by printAndAutoColor
void window::printWordInColor(const char* w, short fg, short bg){
    char* word = (char*)w;
    setcolor(content, fg, bg);
    bool hasSpace = false;
    int wSize = 0;
    for (int i = 0; word[i] != '\0'; i++){
        if (word[i] == ' ')
            hasSpace = true;
        wSize++;
    }
    
    // if the word has a space, print it without the space, then unset color and print the space
    // this is so highlighting words looks correct, only the word and no spaces are highlighted
    if (hasSpace){
        word[wSize-1] = '\0';
        print(word);
        unsetcolor(content, fg, bg);
        print(" ");    
    } else{
        print(word);
        unsetcolor(content, fg, bg);
    }
}

// returns a string with newlines that wraps the text based on this windows size
std::string window::textWrap(std::string s){
    std::string newstr = s;
    std::vector<std::string> lines;

    while (newstr.size() > width){
        int i = width - 1; // start looking at 1 less than the width
        while (newstr[i] != ' '){ // move backwards until you find a space
            i--;
        }
        lines.push_back(newstr.substr(0, i)); // once you find one, save everything before that space
        newstr.erase(0,i+1); // clear that area from the string, repeat the loop
    }
    lines.push_back(newstr); // add remainder

    std::string final = "";
    for (int i = 0; i < lines.size(); i++) // add all the lines back to a single string, now with newlines
    {
        final += lines[i] + "\n";
    }
    
    return final;
}
