#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <src/headerFile/evaluateHand.h>
#include <src/headerFile/screens.h>

extern Texture2D tableImage;


int getCardValue(const Card& card);
int calculateTotal(const vector<Card>& hand, int& tempvalue);
bool BlackJack(const vector<Card>& hand);
bool DoubleAces(const vector<Card>& hand);
void resetGame(Deck& deck,vector<Card> &playerHand, vector<Card> &dealerHand, bool &playerBust, bool &dealerTurn, bool &playerWin5CardRule, int& tempvalue);
bool FiveCardsRule(const vector<Card>& hand);
void GameBlackjack(Deck& deck);


#endif // BLACKJACK_H
