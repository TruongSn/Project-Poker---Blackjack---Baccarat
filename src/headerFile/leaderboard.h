#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <src/headerFile/screens.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>


using namespace std;

extern Texture2D LBBackground;
extern Font titleFont;

void DrawLeaderboard(const vector<UserData>& users, int currentPage, int totalPages);
void HandlePageNavigation(int& currentPage, int totalPages);
void leaderboard();
bool ButtonT(const char* text, int x, int y, int width, int height);
void DrawPlayerDetails(const UserData& user, float x, float y);

#endif // LEADERBOARD_H