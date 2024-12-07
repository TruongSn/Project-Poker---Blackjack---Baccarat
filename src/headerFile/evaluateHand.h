#ifndef EVALUATEHAND_H
#define EVALUATEHAND_H

#include "raylib.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <map>
#include <src/headerFile/Deck.h>

using namespace std;

pair<int, vector<int>> evaluateHand(const Card* hand);

string compareHands(const Card* playerHand, const Card* aiHand);


#endif // EVALUATEHAND_H
