#ifndef BACCARAT_H
#define BACCARAT_H

#include <src/headerFile/Deck.h>
#include <src/headerFile/screens.h>

struct Player {
    vector<Card> hand;
    int score = 0;
};


int CalculateScore(const vector<Card>& hand);
void ResetGame(vector<Player>& players, Deck& deck);
string DetermineWinner(vector<Player>& players);
void DrawGameBaccarat(Deck& deck);

#endif // BACCARAT_H