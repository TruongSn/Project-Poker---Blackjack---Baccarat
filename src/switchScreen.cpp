#include "src/headerFile/Menu.h"
#include <raylib.h>

Rectangle BackButton = {screenWidth - 110, 10, 100, 40};

void DrawBackButton(Sound clickSound){
    Vector2 mousePosition = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePosition, BackButton);

    // Màu sắc cho nút
    Color buttonColor = isHovered ? Fade(DARKBLUE, 0.5f) : RED;
    Color textColor = isHovered ? RED : RAYWHITE;

    DrawRectangleRounded(BackButton, 0.2f, 4, buttonColor);
    DrawText("BACK", BackButton.x + (BackButton.width - MeasureText("BACK", 20)) / 2,
             BackButton.y + (BackButton.height - 20) / 2, 20, textColor);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), BackButton)) {
            PlaySound(clickSound);
        }
    }
}

void UpdateScreen(GameScreen currentScreen){
    // Cập nhật màn hình hiện tại
    switch (currentScreen) {
        case SIGNIN_SCREEN:
            UpdateLoginScreen();
            break;
        case SIGNUP_SCREEN:
            UpdateSignupScreen();
            break;
        case MAIN_GAME_SCREEN:
            break;
    }
}

void DrawScreen(GameScreen currentScreen, Deck& deck, bool& startPvE, bool& startPvP){
    // Vẽ màn hình hiện tại
    switch (currentScreen) {
        case SIGNIN_SCREEN:
            DrawLoginScreen();
            break;
        case SIGNUP_SCREEN:
            DrawSignupScreen();
            break;
        case MENU_GAME_SCREEN:
            DrawMenuGame(clickSound);
            break;
        case BJ_GAME_SCREEN:
            GameBlackjack(deck);
            break;
        case MAIN_GAME_SCREEN:
            DrawMainGame (deck, startPvE, startPvP);
            break;
        case BACCARAT_GAME_SCREEN:
            DrawGameBaccarat(deck);
            break;
        case LEADERBOARD_SCREEN:
            leaderboard();
            break;
        default:
            break;
    }
}