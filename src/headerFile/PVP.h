#ifndef PVP_H
#define PVP_H

#include "raylib.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <map>
#include <src/headerFile/evaluateHand.h>
#include <src/headerFile/screens.h>

extern Sound clickSound;
extern Texture2D tableImage;


string determineWinner(const vector<vector<Card>>& playersHands);
void StartPvPGame(Deck& deck, int playerCount);
int SelectPlayerCount(int screenWidth, int screenHeight);


#endif // PVP_H
