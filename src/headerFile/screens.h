#ifndef SCREENS_H
#define SCREENS_H

#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <string>
#include <stdio.h>
#include <src/headerFile/Deck.h>

extern Rectangle BackButton;

const int MAX_INPUT_LENGTH = 32;
extern Texture2D menuBackground;


// Định nghĩa các trạng thái màn hình
enum GameScreen {
    SIGNUP_SCREEN,
    SIGNIN_SCREEN,
    MENU_GAME_SCREEN,
    MAIN_GAME_SCREEN,
    BJ_GAME_SCREEN,
    BACCARAT_GAME_SCREEN,
    LEADERBOARD_SCREEN

};


extern GameScreen currentScreen;

struct UserData {
    char username[MAX_INPUT_LENGTH + 1] = "";
    char password[MAX_INPUT_LENGTH + 1] = "";
    int money = 1000; // Thêm tiền khởi tạo
    int countMatch = 0;
    int countWin = 0;
    float winRate = 0.0f; // Tỷ lệ thắng 
};

extern UserData userData; 

struct GameState {
    UserData userData;
    bool enteringUsername;
    bool showError;
    const char* errorMessage;
    float errorTimer;
};

void DrawBackButton(Sound clickSound);

//Xử lí dữ liệu trong file
void UpdatePlayerMoneyToFile();

// Khai báo các hàm cho các màn hình
void UpdateLoginScreen();
void DrawLoginScreen();

void InitSignupScreen();
void UpdateSignupScreen();

void DrawSignupScreen();
void UnloadSignupScreen();


void UpdateScreen(GameScreen currentScreen);
void DrawScreen(GameScreen currentScreen, Deck& deck, bool& startPvE, bool& startPvP);

#endif // SCREENS_H
