#include "level.h"

#include <ncursesw/ncurses.h>
#include <cstdlib>
#include <chrono>
#include "window.h"
#include "game.h"
#include "hand.h"
#include "card.h"

level::level(game* g) {
    this->g = g;
    plays = g->getPlays();
    discards = g->getDiscards();
    d = deck(g->d);
    d.shuffle();
    currentScore = 0;
    recentScore = 0;

    int antebases[9] = {100, 300, 800, 2000, 5000, 11000, 20000, 35000, 50000};
    threshold = antebases[g->ante] * (g->round+1)/2;

    for (int i = 0; i < 8; i++) draw();
}

void level::draw() {
    if(d.cards.empty()) return;
    h.add(d.cards.back());
    d.cards.pop_back();
}

void level::playHand() {
    if (plays == 0) return;
    if (h.cardsSelected() == 0){
        recentScore = 0;
        return;
    } 
    played = hand(h.popSelected());
    played.cursor = -1;
    recentScore = calculateScore(&played, &h); //add score of hand
    for (int i = 0; i < played.selected.size(); i++) draw();
    plays--;
}

void level::discardHand() {
    if (discards == 0) return;
    recentScore = 0;
    if (h.cardsSelected() == 0) return;
    played = hand(h.popSelected());
    played.cursor = -1;
    for (int i = 0; i < played.selected.size(); i++) draw();
    played = hand(); // "play" an empty hand
    discards--;
}

int level::calculateScore(hand* played, hand* h){
    std::vector<card> scoredCards = played->scoreCards();
    handtype type = played->pokerHand;
    int flat = g->handTable[type][FLAT];
    int mult = g->handTable[type][MULT];
    g->handTable[type][TIMES_PLAYED] += 1;
    int score = flat;

    std::vector<card> glassCards;

    for (int i = 0; i < scoredCards.size(); i++)
    {
        card c = scoredCards[i];
        enhancement e = c.cardEnhancement;

        if (e == STONE_CARD) 
            score += 50; // if there's a stone card, add 50 instead of card value        
        else 
            score += c.cardValue; // base score of the card

        switch (e){
        case BONUS_CARD:
            score += 30;
            break;
        case MULT_CARD:
            mult += 4;
            break;
        case GLASS_CARD:
            mult *= 2;
            srand(time(0));
            if (rand() % 2 == 0) // 1 in 2 chance
                glassCards.push_back(c);
            break;
        case LUCKY_CARD:
            srand(time(0));
            if (rand() % 5 == 0){ // 1 in 5 chance
                mult += 20;
            }
            if (rand() % 15 == 0){ // 1 in 15 chance
                g->money += 20;
            }
            break;
        }
    }

    // "held in hand" conditions
    for (card c : h->cards){
        enhancement e = c.cardEnhancement;
        switch(e){
            case GOLD_CARD:
                g->money += 3;
                break;
            case STEEL_CARD:
                mult *= 1.5;
        }
    }

    // TODO: destroying glass cards
    for (card c : glassCards){
        g->d.removeCard(c); // need some way to tell the user that the card broke
    }

    recentChips = score;
    recentMult = mult;

    score *= mult;
    currentScore += score;

    return score;
}

void level::win() {
    // earn round money
    int handmoney = plays;
    int interest = g->money / 5;
    int roundreward = g->round + 2;
    int earnings = handmoney + interest + roundreward;

    // display win popup
    window winPopup = window(8, 30, 2, 35, "", g);
    winPopup.print("Cash Out: $%d\nBlind Reward: $%d\n", earnings, roundreward);
    if(handmoney > 0) winPopup.print("Remaining Hands: $%d\n", handmoney);
    if(interest > 0) winPopup.print("Interest: $%d\n", interest);
    wrefresh(winPopup.content);
    getchar();
    delwin(winPopup.content);
    delwin(winPopup.frame);

    // add money on cash out
    g->money += earnings;
    g->gameInfo.updateGameInfo();
}

void level::lose() {
    window lossPopup = window(4, 40, 8, 30, "", g);
    lossPopup.print("YOU LOSE (you ran out of hands)\nPress any key to quit\n");
    wrefresh(lossPopup.content);
    getchar();
    delwin(lossPopup.content);
    delwin(lossPopup.frame);
}
