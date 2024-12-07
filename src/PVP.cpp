#include <src/headerFile/Menu.h>


// Xác định người chiến thắng giữa các người chơi
string determineWinner(const vector<Card*>& playersHands) {
    vector<pair<int, vector<int>>> handRanks;
    for (const auto& hand : playersHands) {
        handRanks.push_back(evaluateHand(hand));
    }

    int winner = 0;
    for (size_t i = 1; i < handRanks.size(); i++) {
        
        if (handRanks[i] > handRanks[winner]) {
            winner = i;
        }
    }

    return "Player " + to_string(winner + 1) + " wins!";
}

// Chức năng cho trò chơi PvP
void StartPvPGame(Deck& deck, int   playerCount) {
    if (playerCount < 0) return;
    
    vector<vector<Card>> playersHands(playerCount);
    vector<int> playerMoney(playerCount, 1000); // Mỗi người chơi bắt đầu với $1000
    vector<bool> isFolded(playerCount, false); // Theo dõi những người chơi đã bỏ bài
    int countFold = 0;
    int currentBet = 0;        // Theo dõi số tiền cược hiện tại
    int previousRaise = 0;     // Theo dõi số tiền của lần cược tăng gần nhất
    int pot = 0;               // Tổng tiền trong pot
    int currentPlayer = 0;     // Theo dõi người chơi hiện tại
    bool gameEnded = false;    // Theo dõi xem trò chơi đã kết thúc chưa
    bool showdown = false;     // Theo dõi xem pha "showdown" có đang diễn ra không
    string winnerText = "";    // Lưu trữ thông tin người chiến thắng
    bool raiseMode = false;    // Theo dõi xem chế độ cược tăng có đang được kích hoạt không
    string raiseInput = "";    // Lưu trữ số tiền cược tăng mà người chơi nhập vào
    string errorMessage = "";  // Lưu trữ thông báo lỗi nếu có nhập sai

    int cardWidth = 100;
    int cardSpacing = 20;
    int startXPos = (screenWidth - (cardWidth + cardSpacing) * 5) / 2;

    // Chia 5 lá bài cho mỗi người chơi
    for (int i = 0; i < playerCount; i++) {
        for (int j = 0; j < 5; j++) {
            playersHands[i].push_back(deck.drawCard());
            DefaultCard(playersHands[i][j], screenWidth / 2, screenHeight + 270);
        }
    }

    // Hàm trợ giúp để chuyển sang người chơi tiếp theo còn hoạt động
    auto moveToNextPlayer = [&]() {
        do {
            currentPlayer = (currentPlayer + 1) % playerCount;
            for (int i = 0; i < 5; i++) 
                DefaultCard(playersHands[currentPlayer][i], screenWidth / 2, screenHeight + 270);
        } while (isFolded[currentPlayer]); // Bỏ qua những người chơi đã Fold
    };

    // Hàm trợ giúp để xác định người chiến thắng
    auto determineWinner = [&]() {
        vector<pair<int, vector<int>>> handRanks;
        int bestPlayer = -1;

        // Kiểm tra xem nếu chỉ còn 1 người chơi chưa fold
        if (countFold == playerCount - 1) {
            // Tìm người chơi duy nhất còn lại chưa fold
            for (int i = 0; i < playerCount; i++) {
                if (!isFolded[i]) {
                    bestPlayer = i;
                    break;
                }
            }
        } else {
            // Nếu không, tiếp tục đánh giá tay bài của các người chơi
            for (int i = 0; i < playerCount; i++) {
                if (!isFolded[i]) {
                    handRanks.push_back(evaluateHand(&playersHands[i][0]));  
                } 
                else {
                    handRanks.push_back({0, {}}); // Những người chơi đã Fold không có thứ hạng
                }
            }

            pair<int, vector<int>> bestHand = {0, {}};
            for (int i = 0; i < playerCount; i++) {
                if (!isFolded[i] && handRanks[i] > bestHand) {
                    bestHand = handRanks[i];
                    bestPlayer = i;
                }
            }
        }

        // Hiển thị kết quả
        if (bestPlayer != -1) {
            winnerText = "Player " + to_string(bestPlayer + 1) + " wins the pot of $" + to_string(pot) + "!";
        } else {
            winnerText = "No winner!";
        }
        gameEnded = true;
    };


    while (!IsKeyPressed(KEY_ESCAPE) && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(tableImage, Rectangle{0, 0, float(tableImage.width), float(tableImage.height)}, Rectangle{0, 0, float(screenWidth), float(screenHeight)}, Vector2{0, 0}, 0, WHITE);

        if (gameEnded) {
            // Hiển thị thông tin người chiến thắng khi trò chơi kết thúc
            DrawText(winnerText.c_str(), screenWidth / 2 - MeasureText(winnerText.c_str(), 30) / 2, screenHeight / 2, 30, YELLOW);
            DrawText("Press [ENTER] to restart", screenWidth / 2 - MeasureText("Press [ENTER] to restart", 20) / 2, screenHeight - 50, 20, WHITE);

            if (IsKeyPressed(KEY_ENTER)) {
                return; // Bắt đầu lại game mới
            }
        } else if (showdown) {
            // Hiển thị tất cả bài của người chơi trong pha showdown
            for (int i = 0; i < playerCount; i++) {
                if (!isFolded[i]) {
                    DrawText(("Player " + to_string(i + 1)).c_str(), startXPos, screenHeight - cardWidth - 300 - i * 120, 20, WHITE);
                    for (size_t j = 0; j < playersHands[i].size(); j++) {
                        deck.updateCard1(playersHands[i][j], startXPos + j * (cardWidth + cardSpacing), screenHeight - cardWidth - 270 - i * 120);
                        deck.showCardImage(playersHands[i][j]);
                    }
                }
            }
            // Xác định và hiển thị người chiến thắng sau pha showdown
            determineWinner();
        } else if (raiseMode) {
            // Hiển thị lời nhắc nhập cược tăng
            DrawText("Enter raise amount:", screenWidth / 2 - MeasureText("Enter raise amount:", 20) / 2, screenHeight / 2 - 60, 20, WHITE);
            DrawText(raiseInput.c_str(), screenWidth / 2 - MeasureText(raiseInput.c_str(), 20) / 2, screenHeight / 2 - 20, 20, YELLOW);

            if (!errorMessage.empty()) {
                DrawText(errorMessage.c_str(), screenWidth / 2 - MeasureText(errorMessage.c_str(), 20) / 2, screenHeight / 2 + 20, 20, RED);
            }

            if (IsKeyPressed(KEY_BACKSPACE) && !raiseInput.empty()) {
                raiseInput.pop_back();
            }

            int key = GetCharPressed();
            if (key >= '0' && key <= '9') {
                raiseInput += static_cast<char>(key);
            }

            if (IsKeyPressed(KEY_ENTER)) {
                int raiseAmount = stoi(raiseInput);
                int minRaise = previousRaise > 0 ? previousRaise : currentBet;
                int maxRaise = playerMoney[currentPlayer]; // Không giới hạn: người chơi có thể cược tất cả (all-in)

                if (raiseAmount < minRaise) {
                    errorMessage = "Raise must be at least $" + to_string(minRaise) + "!";
                    raiseInput = "";
                } else if (raiseAmount > maxRaise) {
                    errorMessage = "Raise cannot exceed your current money!";
                    raiseInput = "";
                } else {
                    currentBet += raiseAmount;
                    pot += raiseAmount;
                    playerMoney[currentPlayer] -= raiseAmount;
                    previousRaise = raiseAmount;
                    raiseMode = false;
                    errorMessage = "";
                    moveToNextPlayer();
                }
            }
        } else {
            // Hiển thị trạng thái người chơi hiện tại và trạng thái trò chơi
            Rectangle Box = {screenWidth / 4, 20, screenWidth / 2, 140};
            DrawRectangleRounded(Box, 0.2f, 5, Fade(DARKGRAY, 0.8f)); // Nền cho hộp trạng thái
            DrawText(("Player " + to_string(currentPlayer + 1) + "'s Turn").c_str(), screenWidth / 2 - MeasureText(("Player " + to_string(currentPlayer + 1) + "'s Turn").c_str(), 30) / 2, 30, 30, WHITE);
            DrawText(("Money: $" + to_string(playerMoney[currentPlayer])).c_str(), screenWidth / 2 - MeasureText(("Money: $" + to_string(playerMoney[currentPlayer])).c_str(), 30) / 2, 60, 30, WHITE);
            DrawText(("Pot: $" + to_string(pot)).c_str(), screenWidth / 2 - MeasureText(("Pot: $" + to_string(pot)).c_str(), 30) / 2, 90, 30, WHITE);
            DrawText(("Current Bet: $" + to_string(currentBet)).c_str(), screenWidth / 2 - MeasureText(("Current Bet: $" + to_string(currentBet)).c_str(), 30) / 2, 120, 30, WHITE);

            if (!isFolded[currentPlayer]) {
                // Hiển thị các lá bài của người chơi hiện tại
                vector<Card>& currentHand = playersHands[currentPlayer];
                for (size_t i = 0; i < currentHand.size(); i++) {
                    deck.updateCard1(currentHand[i], startXPos + i * (cardWidth + cardSpacing), screenHeight - cardWidth - 200);
                    deck.showCardImage(currentHand[i]);
                }

                // Nút hành động với hiệu ứng hover
                const int buttonWidth = 140;
                const int buttonHeight = 50;
                const int spacing = 30;

                const int buttonYPosition = screenHeight - 500; 

                Rectangle checkButton = {screenWidth / 2 - buttonWidth * 2.5f - spacing * 2, buttonYPosition, buttonWidth, buttonHeight};
                Rectangle callButton = {screenWidth / 2 - buttonWidth * 1.5f - spacing, buttonYPosition, buttonWidth, buttonHeight};
                Rectangle raiseButton = {screenWidth / 2 - buttonWidth / 2, buttonYPosition, buttonWidth, buttonHeight};
                Rectangle foldButton = {screenWidth / 2 + buttonWidth / 2 + spacing, buttonYPosition, buttonWidth, buttonHeight};
                Rectangle showdownButton = {screenWidth / 2 + buttonWidth * 1.5f + spacing * 2, buttonYPosition, buttonWidth, buttonHeight};

                // Vẽ nút với hiệu ứng hover
                Vector2 mousePosition = GetMousePosition();
                if (CheckCollisionPointRec(mousePosition, checkButton)) {
                    DrawRectangleRec(checkButton, DARKGREEN); 
                } else {
                    DrawRectangleRec(checkButton, DARKGRAY);
                }
                DrawText("Check", checkButton.x + buttonWidth / 2 - MeasureText("Check", 20) / 2, checkButton.y + buttonHeight / 2 - 10, 20, WHITE);

                if (CheckCollisionPointRec(mousePosition, callButton)) {
                    DrawRectangleRec(callButton, DARKGREEN);
                } else {
                    DrawRectangleRec(callButton, DARKGRAY);
                }
                DrawText("Call", callButton.x + buttonWidth / 2 - MeasureText("Call", 20) / 2, callButton.y + buttonHeight / 2 - 10, 20, WHITE);

                if (CheckCollisionPointRec(mousePosition, raiseButton)) {
                    DrawRectangleRec(raiseButton, DARKGREEN);
                } else {
                    DrawRectangleRec(raiseButton, DARKGRAY);
                }
                DrawText("Raise", raiseButton.x + buttonWidth / 2 - MeasureText("Raise", 20) / 2, raiseButton.y + buttonHeight / 2 - 10, 20, WHITE);

                if (CheckCollisionPointRec(mousePosition, foldButton)) {
                    DrawRectangleRec(foldButton, DARKGREEN);
                } else {
                    DrawRectangleRec(foldButton, DARKGRAY);
                }
                DrawText("Fold", foldButton.x + buttonWidth / 2 - MeasureText("Fold", 20) / 2, foldButton.y + buttonHeight / 2 - 10, 20, WHITE);

                if (CheckCollisionPointRec(mousePosition, showdownButton)) {
                    DrawRectangleRec(showdownButton, DARKGREEN);
                } else {
                    DrawRectangleRec(showdownButton, DARKGRAY);
                }
                DrawText("Showdown", showdownButton.x + buttonWidth / 2 - MeasureText("Showdown", 20) / 2, showdownButton.y + buttonHeight / 2 - 10, 20, WHITE);

                // Xử lý các cú click chuột vào nút
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    // Check action
                    if (CheckCollisionPointRec(mousePosition, checkButton)) {
                        PlaySound(clickSound);
                        if (currentBet == 0) {
                            moveToNextPlayer();
                        }
                    }
                    // Call action
                    else if (CheckCollisionPointRec(mousePosition, callButton)) {
                        PlaySound(clickSound);
                        if (playerMoney[currentPlayer] >= currentBet) {
                            pot += currentBet;
                            playerMoney[currentPlayer] -= currentBet;
                            moveToNextPlayer();
                        }
                    }
                    // Raise action
                    else if (CheckCollisionPointRec(mousePosition, raiseButton)) {
                        PlaySound(clickSound);
                        raiseMode = true;
                        raiseInput = "";
                    }
                    // Fold action
                    else if (CheckCollisionPointRec(mousePosition, foldButton)) {
                        PlaySound(clickSound);
                        isFolded[currentPlayer] = true;
                        countFold++;
                        moveToNextPlayer();
                    }
                    // Showdown action
                    else if (CheckCollisionPointRec(mousePosition, showdownButton)) {
                        PlaySound(clickSound);
                        showdown = true;
                    }
                }
            }
        }

        // Nút quay lại để thoát về MAIN MENU
        DrawBackButton(clickSound);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(GetMousePosition(), BackButton)) {
                for (int i = 0; i < playerCount; i++)
                    deck.ReturnCardsBJ(playersHands[i]);
                currentScreen = MAIN_GAME_SCREEN;
                break;
            }
        }

        EndDrawing();
    }
}



// Constants cho kích thước nút và kích thước văn bản
const float BUTTON_WIDTH_PLAYER = 70;  // Nút chọn số người chơi lớn hơn
const float BUTTON_HEIGHT_PLAYER = 70;
const float BUTTON_TEXT_SIZE_PLAYER = 30;  // Kích thước chữ lớn

//const float BUTTON_TEXT_SIZE_PLAYER = 20;  // Kích thước chữ nhỏ cho tiêu đề

// Hàm để hiển thị và xử lý việc chọn số lượng người chơi
int SelectPlayerCount(int screenWidth, int screenHeight) {
    int playerCount = 1;

    while (!IsKeyPressed(KEY_ESCAPE) && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        
        // Vẽ background, có thể sử dụng ảnh bàn hoặc màu nền tùy ý
        DrawTexturePro(tableImage, Rectangle{0, 0, float(tableImage.width), float(tableImage.height)}, Rectangle{0, 0, float(screenWidth), float(screenHeight)}, Vector2{0, 0}, 0, WHITE);

        // Tiêu đề: "Select Number of Players (1-8)"
        DrawText("Select Number of Players (1-8)", screenWidth / 2 - MeasureText("Select Number of Players (1-8)", BUTTON_TEXT_SIZE_PLAYER) / 2,
                 screenHeight / 2 - 100, BUTTON_TEXT_SIZE_PLAYER, WHITE);

        // Vẽ các nút chọn số người chơi từ 1 đến 8
        for (int i = 1; i <= 8; i++) {
            float buttonX = screenWidth / 2 - BUTTON_WIDTH_PLAYER / 2 + (i - 5) * 80; // Căn giữa và cách đều
            float buttonY = screenHeight / 2;
            Rectangle button = {buttonX, buttonY, BUTTON_WIDTH_PLAYER, BUTTON_HEIGHT_PLAYER};

            // Kiểm tra nếu chuột hover lên nút
            Vector2 mousePosition = GetMousePosition();
            bool isHovered = CheckCollisionPointRec(mousePosition, button);

            // Màu sắc cho nút
            Color buttonColor = isHovered ? LIGHTGRAY : GRAY;
            Color textColor = isHovered ? DARKBLUE : BLACK;

            // Vẽ nút với hình dạng bo góc và màu sắc thay đổi khi hover
            DrawRectangleRounded(button, 0.2f, 4, buttonColor);
            DrawText(to_string(i).c_str(), buttonX + (BUTTON_WIDTH_PLAYER - MeasureText(to_string(i).c_str(), BUTTON_TEXT_SIZE_PLAYER)) / 2, 
                     buttonY + (BUTTON_HEIGHT_PLAYER - BUTTON_TEXT_SIZE_PLAYER) / 2, BUTTON_TEXT_SIZE_PLAYER, textColor);

            // Kiểm tra nếu nhấp chuột vào nút
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePosition, button)) {
                PlaySound(clickSound); // Phát âm thanh click
                playerCount = i;
                return playerCount; // Trả về số người chơi đã chọn
            }
        }

        // Vẽ nút Back (nếu có)
        DrawBackButton(clickSound);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(GetMousePosition(), BackButton)) {
                currentScreen = MAIN_GAME_SCREEN;
                return -1; // Quay lại màn hình chính
            }
        }

        EndDrawing();
    }

    return playerCount; // Trả vêf giá trị mặc định
}
