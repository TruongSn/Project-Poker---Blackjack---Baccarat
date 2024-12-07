#ifndef MENU_H
#define MENU_H

#include <src/headerFile/PVE.h>
#include <src/headerFile/PVP.h>
#include <src/headerFile/Blackjack.h>
#include <src/headerFile/Baccarat.h>
#include <src/headerFile/leaderboard.h>


// Khai báo biến âm thanh
extern Sound clickSound; // Âm thanh click
extern Texture2D menuBackground;


void DrawButton(Rectangle button, const char* text, Sound clickSound, bool& startGameFlag, int textSize, float buttonWidth, float buttonHeight);
void DrawMenu(bool& startPvE, bool& startPvP ,Sound clickSound);
void DrawMainGame (Deck& deck, bool& startPvE, bool& startPvP);
void DrawMenuGame(Sound clickSound);


#endif // MENU_H
