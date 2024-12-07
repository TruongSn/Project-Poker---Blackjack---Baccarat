#include "src/headerFile/Menu.h"


// Hàm cập nhật số tiền người chơi vào file sau khi kết thúc trò chơi
void UpdatePlayerMoneyToFile() {
    std::ifstream file("infoplayers.txt");
    std::stringstream buffer;
    std::string line;
    std::string newContent;
    bool updated = false;

    // Đọc toàn bộ nội dung file vào bộ nhớ
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string fileUsername, filePassword;

        // Đọc username, password
        if (iss >> fileUsername >> filePassword ) {
            for (int i = 0; fileUsername[i] != '\0'; i++) {
                fileUsername[i] = tolower(fileUsername[i]);
            }
            // Nếu username trùng khớp, cập nhật số tiền
            if (fileUsername == userData.username) {
                newContent += fileUsername + " " + filePassword + " " + std::to_string(userData.money) + " " + std::to_string(userData.countMatch) + " " + std::to_string(userData.countWin)  + " " + std::to_string(userData.winRate) + "\n";
                updated = true;
            } else {
                newContent += line + "\n";  // Giữ nguyên các dòng còn lại
            }
        }
    }
    file.close();

    // Nếu đã cập nhật, ghi lại nội dung mới vào file
    if (updated) {
        std::ofstream outFile("infoplayers.txt");
        outFile << newContent;
        outFile.close();
    }
}

// Hàm hiển thị và kiểm tra kết quả trò chơi PvE với tính năng cược tiền và lựa chọn mức cược
void StartPvEGame(Deck& deck) {
    Card playerHand[5];
    Card aiHand[5];
    int betAmount = 0;      // Số tiền cược
    bool bettingPhase = true; // Bước lựa chọn cược

    for (int i = 0; i < 5; i++) {
        playerHand[i] = deck.drawCard();
        aiHand[i] = deck.drawCard();
    }

    bool gameOver = false;
    string winnerText = "";

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    int cardWidth = 180;
    int cardHeight = 270;
    int cardSpacing = cardWidth / 3;
    int playerYPos = screenHeight - cardHeight - 50;
    int aiYPos = 50;
    int startXPos = screenWidth / 2;
    int aiStartYPos = -cardHeight * 2; 
    int playerStartYPos = screenHeight + cardHeight * 2 - 50;
    int aimXPos = screenWidth / 2 - 2.5f * cardWidth - 2.0f * cardSpacing;


    // Hiển thị các thẻ bài
    for (int i = 4; i >= 0; i--) {
        DefaultCard(playerHand[i], startXPos, playerStartYPos);
        DefaultCard(aiHand[i], startXPos, aiStartYPos);
    }

    double startTime = GetTime();

    // Các mức cược
    int betOptions[] = {10, 20, 50, 100, 500};
    int betOptionCount = sizeof(betOptions) / sizeof(betOptions[0]);
    int selectedBetOption = -1;  // Mức cược chưa được chọn
    bool notBetting = false;


    while (!IsKeyPressed(KEY_ESCAPE) && !WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(tableImage, Rectangle{0, 0, (float)tableImage.width, (float)tableImage.height}, Rectangle{0, 0, float(screenWidth), float(screenHeight)}, Vector2{0, 0}, 0.0f, WHITE);

        DrawText("Player", aimXPos, playerYPos - 30, 20, YELLOW);
        DrawText("AI", aimXPos, aiYPos - 30, 20, YELLOW);

        // Hiển thị các thẻ bài
        for (size_t i = 0; i < 5; i++) {
            int x = aimXPos + i * (cardWidth + cardSpacing);
            int y = playerYPos;
            deck.updateCard1(playerHand[i], x, y);
            deck.showCardImage(playerHand[i]);
        }

        for (size_t i = 0; i < 5; i++) {
            int x = aimXPos + i * (cardWidth + cardSpacing);
            int y = aiYPos;
            deck.updateCard2(aiHand[i], x, y);
            deck.showCardImage(aiHand[i]);
        }

        // Hiển thị khung chứa thông tin tiền cược
        float infoBoxWidth = 160;
        float infoBoxHeight = 70;
        float infoBoxX = screenWidth - infoBoxWidth - 20;
        float infoBoxY = screenHeight / 2 - infoBoxHeight / 2;
        
        // Vẽ khung chứa thông tin
        DrawRectangle(infoBoxX, infoBoxY, infoBoxWidth, infoBoxHeight, Fade(BLUE, 0.5f));
        DrawRectangleLines(infoBoxX, infoBoxY, infoBoxWidth, infoBoxHeight, WHITE);

        // Hiển thị thông tin tiền và tiền cược
        DrawText("Money:", infoBoxX + 10, infoBoxY + 15, 20, Fade(GREEN, 0.7f));
        DrawText(to_string(userData.money).c_str(), infoBoxX + 90, infoBoxY + 15, 20, GREEN);

        DrawText("Bet:", infoBoxX + 10, infoBoxY + 45, 20, Fade(GOLD, 0.7f));
        DrawText(to_string(betAmount).c_str(), infoBoxX + 90, infoBoxY + 45, 20, YELLOW);


        double elapsedTime = GetTime() - startTime;

        // Kiểm tra thắng thua
        if (userData.winRate > 0.45f && !gameOver) {
            if (elapsedTime > 3.0f && compareHands(playerHand, aiHand) == "Player wins!") {
                deck.ReturnCards(aiHand);
                deck.shuffleDeck();
                for (int i = 0; i < 5; i++) {
                    aiHand[i] = deck.drawCard();
                    DefaultCard(aiHand[i], aimXPos + i * (cardWidth + cardSpacing), aiYPos);
                    aiHand[i].rotation = 360.0f;
                }
                startTime = GetTime() - 1.5f;
            }
        }


        if (!gameOver) {
            if (!notBetting) {
                // Hiển thị nút cược tiền
                if (bettingPhase) {
                    float buttonWidth = 200;
                    float buttonHeight = 50;
                    float buttonX = (screenWidth - buttonWidth) / 2;
                    float buttonY = screenHeight / 2 - 25;
                    if (CheckCollisionPointRec(GetMousePosition(), Rectangle{buttonX, buttonY, buttonWidth, buttonHeight}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        bettingPhase = false;  // Chuyển sang bước chọn mức cược
                    }
                    DrawRectangle(buttonX, buttonY, buttonWidth, buttonHeight, BLUE);
                    // Tính toán vị trí để căn giữa dòng chữ trong nút
                    int textWidth = MeasureText("Place Bet", 20);
                    int textX = buttonX + (buttonWidth - textWidth) / 2;
                    int textY = buttonY + (buttonHeight - 15) / 2;
                    // Vẽ chữ "Place Bet"
                    DrawText("Place Bet", textX, textY, 20, WHITE);
                } else {
                    // Hiển thị các mức cược khi nút cược được nhấn
                    float buttonWidth = 150;
                    float buttonHeight = 50;
                    float buttonSpacing = 25;
                    float startX = screenWidth / 2 - 2.5f*buttonWidth - 2*buttonSpacing;
                    for (int i = 0; i < betOptionCount; i++) {
                        float buttonX = startX + i * (buttonWidth + buttonSpacing);
                        float buttonY = screenHeight / 2 - 25;
                        if (CheckCollisionPointRec(GetMousePosition(), Rectangle{buttonX, buttonY, buttonWidth, buttonHeight}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                            // Kiểm tra nếu mức cược không vượt quá số tiền hiện có của người chơi
                            PlaySound(clickSound);
                            if (betOptions[i] <= userData.money) {
                                betAmount = betOptions[i];  // Lựa chọn mức cược
                                bettingPhase = true;        // Quay lại bước cược
                            }
                        }
                        DrawRectangle(buttonX, buttonY, buttonWidth, buttonHeight, (betAmount == betOptions[i]) ? GREEN : BLUE);
                        DrawText(("Bet " + to_string(betOptions[i])).c_str(), buttonX + 50, buttonY + 15, 20, WHITE);
                    }
                }
            } else {
                if (elapsedTime > 1.5f) notBetting = false;
                DrawText("You have to make a bet first!", screenWidth / 2 - MeasureText("You have to make a bet first!", 30) / 2, screenHeight / 2 - 15, 30, YELLOW);
            }
            DrawText("Press [SPACE] to reveal winner", screenWidth / 2 - MeasureText("Press [SPACE] to reveal winner", 20) / 2, screenHeight - 40, 20, WHITE);
        }

        // Kiểm tra khi người chơi ấn [SPACE]
        if (IsKeyPressed(KEY_SPACE) && !gameOver && elapsedTime > 2.0f) {
            winnerText = compareHands(playerHand, aiHand);
            if (betAmount < 1) {
                startTime = GetTime();
                notBetting = true;
            } else {
                if (winnerText == "AI wins!") {
                    userData.money -= betAmount;  // Trừ tiền khi thua
                } else if (winnerText == "Player wins!") {
                    userData.countWin++;
                    userData.money += betAmount;  // Thêm tiền khi thắng
                }
                gameOver = true;
                userData.countMatch++;
                userData.winRate = float(userData.countWin) / float(userData.countMatch);
                UpdatePlayerMoneyToFile();
            }


        }

        if (gameOver) {
            // Hiển thị kết quả và hướng dẫn tiếp theo
            for (int i = 0; i < 5; i++) {
                deck.revealCard(aiHand[i]);
            }

            DrawText(winnerText.c_str(), screenWidth / 2 - MeasureText(winnerText.c_str(), 30) / 2, screenHeight / 2 - 15, 30, YELLOW);
            DrawText("Press [ENTER] to restart", screenWidth / 2 - MeasureText("Press [ENTER] to restart", 20) / 2, screenHeight - 40, 20, WHITE);

            if (userData.money <= 0) {
                userData.money = 100;  // Nếu người chơi hết tiền, khởi tạo lại
                UpdatePlayerMoneyToFile();
            }

            // Khởi tạo lại ván chơi
            if (IsKeyPressed(KEY_ENTER)) {
                startTime = GetTime();
                deck.ReturnCards(playerHand);
                deck.ReturnCards(aiHand);

                deck.shuffleDeck();
                for (int i = 0; i < 5; i++) {
                    playerHand[i] = deck.drawCard();
                    aiHand[i] = deck.drawCard();
                }

                for (int i = 4; i >= 0; i--) {
                    DefaultCard(playerHand[i], startXPos, playerStartYPos);
                    DefaultCard(aiHand[i], startXPos, aiStartYPos);
                }

                gameOver = false;
                winnerText = "";
                betAmount = 0;  // Reset cược khi bắt đầu ván mới
            }
        }

        // Quay lại màn hình chính
        DrawBackButton(clickSound);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(GetMousePosition(), BackButton)) {
                currentScreen = MAIN_GAME_SCREEN;
                deck.ReturnCards(playerHand);
                deck.ReturnCards(aiHand);
                break;
            }
        }

        EndDrawing();
    }

}
