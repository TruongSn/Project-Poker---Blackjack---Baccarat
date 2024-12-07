#include "src/headerFile/Menu.h"

const float BUTTON_WIDTH_LARGE = 250;  // Nút lớn hơn
const float BUTTON_HEIGHT_LARGE = 70;  // Nút lớn hơn
const float BUTTON_TEXT_SIZE_LARGE = 30;  // Kích thước chữ lớn hơn

const float BUTTON_WIDTH_SMALL = 200;  // Nút nhỏ (ví dụ nút Back)
const float BUTTON_HEIGHT_SMALL = 50;  // Nút nhỏ
const float BUTTON_TEXT_SIZE_SMALL = 20;  // Kích thước chữ nhỏ hơn
const float BUTTON_SPACING = 20;

// Hàm vẽ và kiểm tra nhấp chuột cho nút
void DrawButton(Rectangle button, const char* text, Sound clickSound, bool& startGameFlag, int textSize, float buttonWidth, float buttonHeight) {
    // Kiểm tra nếu chuột đang hover lên nút
    Vector2 mousePosition = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePosition, button);

    // Màu sắc cho nút
    Color buttonColor = isHovered ? LIGHTGRAY : DARKBLUE;
    Color textColor = isHovered ? DARKBLUE : WHITE;

    // Vẽ nút với hình dạng bo góc và màu sắc thay đổi khi hover
    DrawRectangleRounded(button, 0.2f, 4, buttonColor);
    DrawText(text, button.x + (button.width - MeasureText(text, textSize)) / 2,
             button.y + (button.height - textSize) / 2, textSize, textColor);

    // Kiểm tra nếu nhấp chuột vào nút
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isHovered) {
        PlaySound(clickSound); // Phát âm thanh click
        startGameFlag = true;
    }
}

// Hàm vẽ menu chính
void DrawMenu(bool& startPvE, bool& startPvP, Sound clickSound) {
    // Tính toán vị trí trung tâm của màn hình
    float centerX = screenWidth / 2;
    float centerY = screenHeight / 2;

    // Tạo các nút PvE và PvP (với kích thước lớn hơn)
    Rectangle pveButton = {centerX - BUTTON_WIDTH_LARGE / 2, centerY - BUTTON_HEIGHT_LARGE - BUTTON_SPACING, BUTTON_WIDTH_LARGE, BUTTON_HEIGHT_LARGE};
    Rectangle pvpButton = {centerX - BUTTON_WIDTH_LARGE / 2, centerY + BUTTON_SPACING, BUTTON_WIDTH_LARGE, BUTTON_HEIGHT_LARGE};

    // Vẽ các nút và kiểm tra nhấp chuột
    DrawButton(pveButton, "PvE", clickSound, startPvE, BUTTON_TEXT_SIZE_LARGE, BUTTON_WIDTH_LARGE, BUTTON_HEIGHT_LARGE);
    DrawButton(pvpButton, "PvP", clickSound, startPvP, BUTTON_TEXT_SIZE_LARGE, BUTTON_WIDTH_LARGE, BUTTON_HEIGHT_LARGE);
}


// Hàm vẽ menu chính
void DrawMenuGame(Sound clickSound) {
    ClearBackground(WHITE);
    DrawTexturePro(menuBackground, 
        Rectangle{0, 0, (float)menuBackground.width, (float)menuBackground.height}, 
        Rectangle{0, 0, screenWidth, screenHeight}, 
        Vector2{0, 0}, 
        0, 
        WHITE);

    // Tính toán vị trí trung tâm của màn hình
    float centerX = screenWidth / 2;
    float centerY = screenHeight / 2;

    bool PK = false;
    bool BJ = false;
    bool BC = false;
    bool LB = false;

    DrawBackButton(clickSound);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), BackButton)) {
            currentScreen = SIGNIN_SCREEN;
            return;
        }
    }

    Rectangle PKButton = {centerX - BUTTON_WIDTH_LARGE / 2, centerY - BUTTON_HEIGHT_LARGE * 5 / 2 - BUTTON_SPACING * 2, BUTTON_WIDTH_LARGE, BUTTON_HEIGHT_LARGE};
    Rectangle BCButton = {centerX - BUTTON_WIDTH_LARGE / 2, centerY - BUTTON_HEIGHT_LARGE * 3 / 2 - BUTTON_SPACING, BUTTON_WIDTH_LARGE, BUTTON_HEIGHT_LARGE};
    Rectangle BJButton = {centerX - BUTTON_WIDTH_LARGE / 2, centerY - BUTTON_HEIGHT_LARGE / 2, BUTTON_WIDTH_LARGE, BUTTON_HEIGHT_LARGE};
    Rectangle LBButton = {centerX - BUTTON_WIDTH_LARGE / 2, centerY + BUTTON_HEIGHT_LARGE / 2 + BUTTON_SPACING, BUTTON_WIDTH_LARGE, BUTTON_HEIGHT_LARGE};

    // Vẽ các nút và kiểm tra nhấp chuột
    DrawButton(PKButton, "Poker", clickSound, PK, BUTTON_TEXT_SIZE_LARGE, BUTTON_WIDTH_LARGE, BUTTON_HEIGHT_LARGE);
    DrawButton(BCButton, "Baccarat", clickSound, BC, BUTTON_TEXT_SIZE_LARGE, BUTTON_WIDTH_LARGE, BUTTON_HEIGHT_LARGE);
    DrawButton(BJButton, "Blackjack", clickSound, BJ, BUTTON_TEXT_SIZE_LARGE, BUTTON_WIDTH_LARGE, BUTTON_HEIGHT_LARGE);
    DrawButton(LBButton, "Leaderboard", clickSound, LB, BUTTON_TEXT_SIZE_LARGE, BUTTON_WIDTH_LARGE, BUTTON_HEIGHT_LARGE);

    if (PK) currentScreen = MAIN_GAME_SCREEN;
    if (BC) currentScreen = BACCARAT_GAME_SCREEN;
    if (BJ) currentScreen = BJ_GAME_SCREEN;
    if (LB) currentScreen = LEADERBOARD_SCREEN;
}


void DrawMainGame (Deck& deck, bool& startPvE, bool& startPvP){
    if (!startPvE && !startPvP) {
        DrawTexturePro(menuBackground, 
            Rectangle{0, 0, (float)menuBackground.width, (float)menuBackground.height}, 
            Rectangle{0, 0, screenWidth, screenHeight}, 
            Vector2{0, 0}, 
            0, 
            WHITE);
        DrawBackButton(clickSound);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(GetMousePosition(), BackButton)) {
                currentScreen = MENU_GAME_SCREEN;
                return;
            }
        }
        DrawMenu(startPvE, startPvP, clickSound); // Truyền âm thanh vào
    } else if (startPvE) {
        StartPvEGame(deck);
        startPvE = false;
    }
    else if (startPvP) {
        int playerCount = SelectPlayerCount(screenWidth, screenHeight);
        StartPvPGame(deck, playerCount);
        startPvP = false;
    }
}