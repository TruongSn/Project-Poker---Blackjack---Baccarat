#include "raylib.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <map>
#include "src/headerFile/Menu.h"


using namespace std;



GameScreen currentScreen = SIGNIN_SCREEN;
Sound clickSound;

Texture2D tableImage;
Texture2D cardImages;
Texture2D cardBack;
Texture2D menuBackground;
Font titleFont;
Texture2D LBBackground;



int main() {


    InitWindow(screenWidth, screenHeight, "Poker Game");
    InitAudioDevice(); // Khởi tạo âm thanh
    SetWindowState(FLAG_VSYNC_HINT);


    menuBackground = LoadTextureFromImage(LoadImage("images/background1.png"));
    // Load mặt bàn chơi
    tableImage = LoadTextureFromImage(LoadImage("images/table.png"));
    // các mặt trước của lá bài
    cardImages = LoadTextureFromImage(LoadImage("images/all_cards.png"));;
    // các mặt sau của lá bài
    cardBack = LoadTextureFromImage(LoadImage("images/backs.png"));;

    clickSound = LoadSound("sound/click_sound.mp3"); // Load âm thanh click

    titleFont = LoadFont("fonts/alagard.ttf");
    LBBackground = LoadTextureFromImage(LoadImage("images/bgleaderboard.png"));

    Deck deck;
    deck.shuffleDeck();

    bool startPvE = false;
    bool startPvP = false;
    SetTargetFPS(60);

    while(!WindowShouldClose()){
        BeginDrawing();
        DrawScreen(currentScreen, deck, startPvE, startPvP);
        UpdateScreen(currentScreen);
        EndDrawing();
    }


    UnloadSound(clickSound); // Giải phóng âm thanh
    UnloadTexture(menuBackground);
    UnloadTexture(tableImage);
    UnloadTexture(cardImages);
    UnloadTexture(cardBack);
    UnloadTexture(LBBackground);
    UnloadFont(titleFont);
    CloseAudioDevice(); // Đóng thiết bị âm thanh
    CloseWindow();

    return 0;
}


