#include "src/headerFile/Menu.h"



// Hàm tính điểm
int CalculateScore(const vector<Card>& hand) {
    int score = 0;
    for (const auto& card : hand) {
        score += card.rank > 10 ? 10 : card.rank; // Át = 1, 2-10 là giá trị tương ứng, J/Q/K = 10
    }
    return score % 10; // Điểm chỉ lấy phần dư
}

// Reset game
void ResetGame(vector<Player>& players, Deck& deck) {
    for (auto& player : players) {
        deck.ReturnCardsBJ(player.hand);
        deck.shuffleDeck();
        for (int i = 0; i < 3; ++i) {
            player.hand.push_back(deck.drawCard());
            DefaultCardBJ(player.hand.back(), screenWidth + 180 * 2, screenHeight / 2); 
        }
        player.score = CalculateScore(player.hand);
    }
}

// Xác định người chiến thắng
string DetermineWinner(vector<Player>& players) {
    string winner = "Dealer wins!";
    if (players[1].score > players[0].score) {
        winner = "Player wins!";
    } else if (players[1].score == players[0].score) {
        winner = "It's a tie!";
    }
    return winner;
}


// Hiển thị giao diện game
void DrawGameBaccarat(Deck& deck) {
    vector<Player> players(2); // Dealer và Player
    ResetGame(players, deck);

    int currentDealerIndex = 0;
    int currentPlayerIndex = 0;

    int cardSpacing = screenWidth / 15;
    int dealerStartY = screenHeight / 10;
    int playerStartY = screenHeight * 6 / 10;

    string winnerMessage = "";
    bool gameOver = false;
    
    float elapsedTime;
    float timer = GetTime();


    while (!IsKeyPressed(KEY_ESCAPE) && !WindowShouldClose()) {
            
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(tableImage, Rectangle{0, 0, (float)tableImage.width, (float)tableImage.height}, Rectangle{0, 0, float(screenWidth), float(screenHeight)}, Vector2{0, 0}, 0.0f, PURPLE);



        elapsedTime = GetTime() - timer;

        Rectangle Reveal = {screenWidth * 7 / 10, screenHeight * 6 / 10 + 40, 200, 70};
        Rectangle Restart = {screenWidth * 7 / 10, screenHeight * 6 / 10 + 130, 200, 70};
        Color ButtonReveal = CheckCollisionPointRec(GetMousePosition(), Reveal) ? ORANGE : Fade(ORANGE, 0.3f);
        Color ButtonRestart = CheckCollisionPointRec(GetMousePosition(), Restart) ? GREEN : Fade(GREEN, 0.3f);
        Color TextReveal = CheckCollisionPointRec(GetMousePosition(), Reveal) ? WHITE : Fade(YELLOW, 0.3f);
        Color TextRestart = CheckCollisionPointRec(GetMousePosition(), Restart) ? WHITE : Fade(YELLOW, 0.3f);


        DrawRectangleRounded(Reveal, 0.2f, 4, ButtonReveal);
        DrawRectangleRoundedLines(Reveal, 0.2f, 4, Fade(WHITE, 0.7f));
        DrawRectangleRounded(Restart, 0.2f, 4, ButtonRestart);
        DrawRectangleRoundedLines(Restart, 0.2f, 4, Fade(WHITE, 0.7f));
        DrawText("Reveal", Reveal.x + (Reveal.width - MeasureText("Reveal", 40)) / 2, Reveal.y + (Reveal.height - 40) / 2, 40, TextReveal);
        DrawText("Restart", Restart.x + (Restart.width - MeasureText("Restart", 40)) / 2, Restart.y + (Restart.height - 40) / 2, 40, TextRestart);

        if (CheckCollisionPointRec(GetMousePosition(), Restart) && elapsedTime > 2.0f 
            && gameOver && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            ResetGame(players, deck);
            gameOver = false;
            winnerMessage = "";
            currentDealerIndex = 0;
            currentPlayerIndex = 0;
            timer = GetTime();
        }

        if (CheckCollisionPointRec(GetMousePosition(), Reveal) && !gameOver 
            && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && elapsedTime > 2.0f) {
            winnerMessage = DetermineWinner(players);
            gameOver = true;
        }

        // Hiển thị bài của Dealer
        DrawText("Dealer", screenWidth / 20, dealerStartY - 30, 20, WHITE);

        if (gameOver && currentDealerIndex > 0) {
            deck.revealCard(players[0].hand[1]);
            deck.revealCard(players[0].hand[2]);
        }
        else deck.revealCard(players[0].hand[0]);
        deck.UpdateCards(players[0].hand, dealerStartY, currentDealerIndex, cardSpacing, 100);
        deck.DrawAllCards(players[0].hand);

        // Hiển thị bài của Player
        DrawText("Player", screenWidth / 20, playerStartY - 30, 20, WHITE);
        deck.UpdateCards(players[1].hand, playerStartY, currentPlayerIndex, cardSpacing, 100);
        if (currentPlayerIndex > 0) deck.revealCard(players[1].hand[currentPlayerIndex - 1]);
        deck.DrawAllCards(players[1].hand);


        // Hiển thị thông tin
        if (!gameOver) DrawText("Dealer Score: ?", screenWidth * 7 / 10, dealerStartY, 20, WHITE);
        DrawText(("Player Score: " + to_string(players[1].score)).c_str(), screenWidth * 7 / 10, playerStartY, 20, WHITE);

        // Hiển thị kết quả nếu game kết thúc
        if (gameOver) {
            DrawText(("Dealer Score: " + to_string(players[0].score)).c_str(), screenWidth * 7 / 10, dealerStartY, 20, WHITE);
            DrawText(winnerMessage.c_str(), screenWidth / 2 - MeasureText(winnerMessage.c_str(), 30) / 2, screenHeight / 2, 30, RED);
        }

        DrawBackButton(clickSound);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(GetMousePosition(), BackButton)) {
                currentScreen = MENU_GAME_SCREEN;
                deck.ReturnCardsBJ(players[1].hand);
                deck.ReturnCardsBJ(players[0].hand);
                break;
            }
        }

        EndDrawing();
    }
}