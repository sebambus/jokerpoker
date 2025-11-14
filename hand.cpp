#include "hand.h"

#include "color.h"
#include "card.h"

const char* handName(handtype h) {
    switch (h) {
    case FIVE_FLUSH:
        return "Flush Five";
    case HOUSE_FLUSH:
        return "Flush House";
    case FIVE_KIND:
        return "Five of a Kind";
    case ROYAL_FLUSH:
        return "Royal Flush";
    case STRAIGHT_FLUSH:
        return "Straight Flush";
    case FOUR_KIND:
        return "Four of a Kind";
    case FULL_HOUSE:
        return "Full House";
    case FLUSH:
        return "Flush";
    case STRAIGHT:
        return "Straight";
    case THREE_KIND:
        return "Three of a Kind";
    case TWO_PAIR:
        return "Two Pair";
    case PAIR:
        return "Pair";
    case HIGH:
        return "High Card";
    default:
        return "Mystery Hand!!!";
    }
}

// creates blank hand
hand::hand() {
    cursor = 0;
}

// creates hand containing given cards
hand::hand(std::vector<card> list) {
    cards = list;
    for(int i = 0; i < cards.size(); i++)   // fills selected with false
        selected.push_back(false);
    cursor = 0;
}

void hand::add(card c) {
    cards.push_back(c);
    selected.push_back(false);
}

// deletes and returns last card in hand
card hand::pop() {
    selected.pop_back();
    card c = cards.back();
    cards.pop_back();
    if(cursor == cards.size()) cursor--;    // ensures cursor remains in hand
    return c;
}

// deletes and returns a vector of all selected cards
std::vector<card> hand::popSelected() {
    std::vector<card> out;

    for(int i = 0; i < cards.size(); i++)
        if(selected[i]) {                   // loops over selected cards
            out.push_back(cards[i]);
            cards.erase(cards.begin()+i);
            selected.erase(selected.begin()+i);
             
            if(cursor >= i) cursor--;   // shifts cursor if needed to stay on same card
            i--;    // repeats loop on current line, since there is now a new card there
        }

    return out;
}

// prints all cards in hand, with cursor highlighted and selected cards shifted
void hand::print(WINDOW* w) {
    int x, y;
    getyx(w, y, x);
    for(int i = 0; i < cards.size(); i++) {
        //highlight cursor
        if(i == cursor) wattron(w, A_REVERSE);
        // shift card up or down based on selection
        if(selected[i])
            wmove(w, y+1, getcurx(w));
        else
            wmove(w, y, getcurx(w));

        cards[i].print(w);
        // reset color attributes
        if(i == cursor) wattroff(w, A_REVERSE);
        setcolor(w, COLOR_WHITE, COLOR_BLACK);
        // spacing
        wprintw(w, " ");
    }
    // movve cursor to beneath
    wmove(w, y+2, x);
}

// moves cursor right by moveBy
void hand::moveCursor(int moveBy){
    int newPosition = cursor + moveBy;
    if (newPosition > cards.size() -1 || newPosition < 0)
        return; //dont move cursor if attempting to move beyond hand
    cursor += moveBy;
}

void hand::selectCursor() {
    if(cursor >= 0 && cursor < selected.size())
        selected[cursor] = !selected[cursor] && cardsSelected() < 5;
}

int hand::cardsSelected() {
    int n = 0;
    for (int i = 0; i < selected.size(); i++)
        if (selected[i] == true)
            n++;
    return n;
}

void hand::sortBySuit(){
    std::vector<card> spades;
    std::vector<card> hearts;
    std::vector<card> diamonds;
    std::vector<card> clubs;

    //sort cards into vectors by suit
    for (int i = 0; i < cards.size(); i++)
    {
        suit cardSuit = cards.at(i).cardSuit;
        switch (cardSuit){
        case SPADE:
            spades.push_back(cards.at(i));
            break;
        case HEART:
            hearts.push_back(cards.at(i));
            break;
        case DIAMOND:
            diamonds.push_back(cards.at(i));
            break;
        case CLUB:
            clubs.push_back(cards.at(i));
            break;
        }        
    }

    //sort each vector by value
    std::vector<card>sortedSpades = subsortByValue(spades);
    std::vector<card>sortedHearts = subsortByValue(hearts);
    std::vector<card>sortedDiamonds = subsortByValue(diamonds);
    std::vector<card>sortedClubs = subsortByValue(clubs);
    std::vector<card> sortedVectors[4] = {sortedSpades, sortedHearts, sortedDiamonds, sortedClubs};

    //add them to hand
    cards.clear();
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < sortedVectors[i].size(); j++)
        {
            cards.push_back(sortedVectors[i].at(j));
        }
        
    }
    return;
}



void hand::sortByValue(){
    std::vector<card> sortedVector;

    for (int i = 1; i < 15; i++) //for each possible value, check each card
    {
        for (int j = 0; j < cards.size(); j++)
        {
            if (cards.at(j).cardValue == i) //if card is that value, add it the sorted vector
                sortedVector.push_back(cards.at(j));
        }
    }
    
    cards.clear();

    for (int i = 0; i < sortedVector.size(); i++)
    {
        cards.push_back(sortedVector.at(i));
    }
    

    return;
}

//helper functions for sorting.
//take vectors split up with one method, then sort them using the other
std::vector<card> hand::subsortByValue(std::vector<card> sub){ //sorts each suit group by value
    std::vector<card> sortedVector;
    for (int i = 1; i < 15; i++) //for each possible value, check each card
    {
        for (int j = 0; j < sub.size(); j++)
        {
            if (sub.at(j).cardValue == i) //if card is that value, add it the sorted vector
                sortedVector.push_back(sub.at(j));
        }
    }

    return sortedVector;
}

std::vector<card> hand::subsortBySuit(std::vector<card> sub){
    return sub;
}


void hand::swapSelected(){
    if (cardsSelected() != 2) return; //if more than or less than 2 cards are selected
    std::vector<int> positions;
    for (int i = 0; i < selected.size(); i++){
        if (selected[i]){
            positions.push_back(i);
        }
    }

    //swap cards
    card c1 = cards[positions[0]];
    card c2 = cards[positions[1]];

    cards[positions[0]] = c2;
    cards[positions[1]] = c1;

    for (int x : positions) //unselect the selected cards
        selected[x] = false;
}


//returns poker hand and cards within hand that should be scored
std::vector<card> hand::scoreCards(){
    std::vector<card> scoredCards;
    
    // finding stone cards. they are always scored, so remove them before running the algorithm
    // add them to the list of cards to be scored
    std::vector<card> stoneCards;
    for (int i = 0; i < cards.size(); i++){
        if (cards[i].cardEnhancement == STONE_CARD){
            stoneCards.push_back(cards[i]);
            scoredCards.push_back(cards[i]);
            cards.erase(cards.begin() + i);
        }
    }
    
    // counting suits and ranks
    int suitCount[4] = { 0 };
    int rankCount[13] = { 0 };
    for(card c : cards) {
        if (c.cardEnhancement == WILD_CARD){
            for (int i = 0; i < 4; i++) suitCount[i]++;
        }
        else 
            suitCount[(int) c.cardSuit]++;
        rankCount[c.cardValue-1]++;
    }

    // finds top two rank counts
    int max = 0;
    int next = 0;
    for(int x : rankCount) {
        if(max < x) {
            next = max;
            max = x;
        }
        else if(next < x)
            next = x;
    }

    int highestScoredValue = 0;
    int secondHighestValue = 0;

    for (int i = 0; i < 14; i++){
        if (rankCount[i] == max){
            if(highestScoredValue != 0){
                secondHighestValue = i+1;
            } else {
                highestScoredValue = i+1;
            }
        }
    }

    // add stone cards back to cards
    for (card c : stoneCards){
        cards.push_back(c);
    }

    // finds flush
    bool flush = false;
    for(int x : suitCount)
        if(x == 5)
            flush = true;

    bool straight = false;
    bool royal = false;
    // finds straight/royal straight TODO
    royal = rankCount[0] * rankCount[12] * rankCount[11] * rankCount[10] * rankCount[9] != 0;
    straight = royal;
    for(int i = 0; i < 9; i++)
        if(rankCount[i] * rankCount[i+1] * rankCount[i+2] * rankCount[i+3] * rankCount[i+4] != 0)
            straight = true;

    if(flush && max == 5){pokerHand = FIVE_FLUSH; return cards;}
    if(flush && max == 3 && next == 2){ pokerHand = HOUSE_FLUSH; return cards;}
    if(max == 5){ pokerHand = FIVE_KIND; return cards;}
    if(royal && straight && flush){ pokerHand = ROYAL_FLUSH; return cards;}
    if(straight && flush){ pokerHand = STRAIGHT_FLUSH; return cards;}
    if(max == 4){
        for (card c : cards){
            if (c.cardValue == highestScoredValue){
                scoredCards.push_back(c);
            }
        }
        pokerHand = FOUR_KIND; return scoredCards;
    } 
    if(max == 3 && next == 2){ pokerHand = FULL_HOUSE; return cards;}
    if(flush){ pokerHand = FLUSH; return cards;}
    if(straight){ pokerHand = STRAIGHT; return cards;}
    if(max == 3){
        for (card c : cards){
            if (c.cardValue == highestScoredValue){
                scoredCards.push_back(c);
            }
        }
        pokerHand = THREE_KIND; return scoredCards;
    }
    if(max == 2 && next == 2){
        for (card c : cards){
            if (c.cardValue == highestScoredValue || c.cardValue == secondHighestValue)
                scoredCards.push_back(c);
        }
        pokerHand = TWO_PAIR; return scoredCards;
    } 
    if(max == 2){         
        for (card c : cards){
            if (c.cardValue == highestScoredValue){
                scoredCards.push_back(c);
            }
        }
        pokerHand = PAIR; return scoredCards;
    }
    else{
        card greatestCard = cards[0];

        for (card c : cards){
            if (c.cardValue > greatestCard.cardValue)
                greatestCard = c;
        }
        scoredCards.push_back(greatestCard);
        pokerHand = HIGH; return scoredCards;
    }
   
}