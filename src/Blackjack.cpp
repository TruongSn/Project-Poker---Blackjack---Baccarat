#include <src/headerFile/Menu.h>

// Tính giá trị của lá bài dựa vào chỉ số file ảnh
int getCardValue(const Card& card) {
    int value = card.rank; // 2–14 (A = 14)
    if (value == 14) return 1;           // A = 1 (có thể đổi thành 11 sau)
    if (value > 10) return 10;           // J/Q/K = 10
    return value;
}



// Tính tổng điểm các lá bài
int calculateTotal(const vector<Card>& hand, int& tempvalue) {
    int total = 0;
    int aceCount = 0;

    for (Card card : hand) {
        int value = getCardValue(card);
        if (value == 1) aceCount++; // Đếm số Ách
        total += value;
    }

    // Chuyển Ách từ 1 điểm thành 11 điểm nếu tổng <= 11
    while (total <= 11 && aceCount > 0) {
        tempvalue = total;
        total += 10; // Chuyển Ách thành 11
        aceCount--;
    }

    return total;
}

bool BlackJack(const vector<Card>& hand) {
    return hand.size() == 2 && ((getCardValue(hand[0]) == 1 && getCardValue(hand[1]) == 10)
                            || (getCardValue(hand[0]) == 10 && getCardValue(hand[1]) == 1));
}

bool DoubleAces(const vector<Card>& hand){
    return hand.size() == 2 && (getCardValue(hand[0]) == 1 && getCardValue(hand[1]) == 1);
}

// Hàm để reset trò chơi
void resetGame(Deck& deck,vector<Card> &playerHand, vector<Card> &dealerHand, bool &playerBust, bool &dealerTurn, bool &playerWin5CardRule, int& tempvalue) {
    float startXPos = screenWidth + 180;
    float StartYPos = screenHeight/2;

    tempvalue = 0;
    deck.ReturnCardsBJ(playerHand);
    deck.ReturnCardsBJ(dealerHand);

    playerBust = false;
    dealerTurn = false;
    playerWin5CardRule = false;
    deck.shuffleDeck();

    // Tạo bài ban đầu cho người chơi và nhà cái
    playerHand.push_back(deck.drawCard());
    playerHand.push_back(deck.drawCard());

    dealerHand.push_back(deck.drawCard());
    dealerHand.push_back(deck.drawCard());

    for (Card& card : playerHand) {
        DefaultCardBJ(card, startXPos, StartYPos);
    }
    for (Card& card : dealerHand) {
        DefaultCardBJ(card, startXPos, StartYPos);
    }
}

bool FiveCardsRule(const vector<Card>& hand) {
    int temp = 0;
    if (hand.size() == 5 && calculateTotal(hand, temp) < 22)
        return true;
    return false;
}

void GameBlackjack(Deck& deck){
    // Các biến lưu bài
    vector<Card> playerHand;
    vector<Card> dealerHand;

    int tempvalue = 0;

    float cardWidth = 180;
    float cardHeight = 270;

    // Trạng thái trò chơi
    bool playerBust = false;
    bool dealerTurn = false;
    bool playerWin5CardRule = false; // Biến để kiểm tra nếu thắng do 5 lá bài

    int cardSpacing = 0;

    float StartXPos = screenWidth + cardWidth;
    float StartYPos = screenHeight/2;

    int aimXPos = (screenWidth - (cardWidth + cardSpacing) * 5) / 2 - 100;

    bool LowPoint = false;
    double startTime = GetTime();
    double elapsedTime;

    int currentDealerCardIndex = 0; // Lá bài của dealer đang di chuyển
    int currentPlayerCardIndex = 0; // Lá bài của player đang di chuyển


    // Khởi tạo ván chơi mới
    resetGame(deck, playerHand, dealerHand, playerBust, dealerTurn, playerWin5CardRule, tempvalue);

    while (!IsKeyPressed(KEY_ESCAPE) && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK); // Màu nền đen
        DrawTexturePro(tableImage, Rectangle{0, 0, (float)tableImage.width, (float)tableImage.height}, Rectangle{0, 0, float(screenWidth), float(screenHeight)}, Vector2{0, 0}, 0.0f, BLUE);


        // Hiển thị bài của người chơi
        DrawText("Your cards:", 50, screenHeight - cardHeight - 50, 30, WHITE); // Chữ màu trắng
        if (tempvalue) DrawText(("Total points: " + to_string(tempvalue) + "/" + to_string(calculateTotal(playerHand, tempvalue))).c_str(), 50 + cardWidth * 2, screenHeight - cardHeight - 40, 30, WHITE);
        else DrawText(("Total points: " + to_string(calculateTotal(playerHand, tempvalue))).c_str(), 50 + cardWidth * 2, screenHeight - cardHeight - 50, 30, WHITE); 

        deck.UpdateCards(playerHand, screenHeight - cardHeight - 10, currentPlayerCardIndex, cardSpacing, aimXPos);
        if (currentPlayerCardIndex > 0) deck.revealCard(playerHand[currentPlayerCardIndex - 1]);
        deck.DrawAllCards(playerHand);

        int aiValue = 0;
        // Hiển thị bài của nhà cái
        DrawText("Dealer's cards:", 50, 10, 30, WHITE);
        DrawText(("Total points: " + string(dealerTurn ? to_string(calculateTotal(dealerHand, aiValue)) : "?")).c_str(), 50 + cardWidth * 2, 10, 30, WHITE);

        if (dealerTurn && currentDealerCardIndex > 0) deck.revealCard(dealerHand[currentDealerCardIndex - 1]);
        else deck.revealCard(dealerHand[0]);

        deck.UpdateCards(dealerHand, 50, currentDealerCardIndex, cardSpacing, aimXPos);
        deck.DrawAllCards(dealerHand);

        // Hiển thị các nút
        // Nút Hit
        Rectangle Hit = {screenWidth - 200, cardHeight, 200, 80};
        bool mouseOverHit = CheckCollisionPointRec(GetMousePosition(), Hit);
        Color hitColor = mouseOverHit ? Fade(BLUE, 0.5f) : BLUE;
        Color hitTextColor = mouseOverHit ? Fade(WHITE, 0.7f) : WHITE;
        DrawRectangleRec(Hit, hitColor);
        DrawRectangleLinesEx(Hit, 1, WHITE);
        DrawText("Hit", Hit.x + (Hit.width - MeasureText("Hit", 30)) / 2,
                                    Hit.y + (Hit.height - 30) / 2, 30, hitTextColor);

        // Nút stand
        Rectangle Stand = {screenWidth - 200, cardHeight + 80, 200, 80};
        bool mouseOverStand = CheckCollisionPointRec(GetMousePosition(), Stand);
        Color standColor = mouseOverStand ? Fade(RED, 0.5f) : RED;
        Color standTextColor = mouseOverStand ? Fade(WHITE, 0.7f) : WHITE;
        DrawRectangleRec(Stand, standColor);
        DrawRectangleLinesEx(Stand, 1, WHITE);
        DrawText("Stand", Stand.x + (Stand.width - MeasureText("Stand", 30)) / 2,
                                    Stand.y + (Stand.height - 30) / 2, 30, standTextColor);

        // Nút restart
        Rectangle Restart = {screenWidth - 200, cardHeight + 160, 200, 80};
        bool mouseOverRestart = CheckCollisionPointRec(GetMousePosition(), Restart);
        Color restartColor = mouseOverRestart ? Fade(GREEN, 0.5f) : GREEN;
        Color restartTextColor = mouseOverRestart ? Fade(WHITE, 0.7f) : WHITE;
        DrawRectangleRec(Restart, restartColor);
        DrawRectangleLinesEx(Restart, 1, WHITE);
        DrawText("Restart", Restart.x + (Restart.width - MeasureText("Restart", 30)) / 2,
                                    Restart.y + (Restart.height - 30) / 2, 30, restartTextColor);

        if (LowPoint) DrawText("Your points are less than 16, you must hit!", 50, screenHeight / 2, 30, RED);
        if (playerBust && !dealerTurn) DrawText("Your points are more than 21, you can't draw another card!", 50, screenHeight / 2, 30, RED);
        if (playerWin5CardRule && !dealerTurn) DrawText("You can't draw another card, you must stand!", 50, screenHeight / 2, 30, RED);


        if (BlackJack(playerHand) && !dealerTurn) DrawText("You got Blackjack! You should stand now!", 50, screenHeight / 2, 30, GREEN);
        if (DoubleAces(playerHand) && !dealerTurn) DrawText("You got Two Aces! You should stand now!", 50, screenHeight / 2, 30, GREEN);

        elapsedTime = GetTime() - startTime;

        // Xử lý logic trò chơi 
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && elapsedTime > 2.0f) {
            Vector2 mousePoint = GetMousePosition();

            // Xử lý khi người chơi bấm nút "Rút bài"
            if (CheckCollisionPointRec(mousePoint, Hit) && !dealerTurn) {
                LowPoint = false;
                tempvalue = 0;

                
                if (playerHand.size() < 5 && !playerBust) { // Chỉ bốc thêm bài nếu chưa đủ 5 lá
                    playerHand.push_back(deck.drawCard());
                    DefaultCardBJ(playerHand.back(), StartXPos, StartYPos);
                    startTime = GetTime() - 1.0f;

                    if (calculateTotal(playerHand, aiValue) > 21) {
                        playerBust = true; // Người chơi bị quắc
                    }
                }
            }

            // Xử lý khi người chơi bấm nút "Dừng"
            if (CheckCollisionPointRec(mousePoint, Stand)) {
                int playerTotal = calculateTotal(playerHand, tempvalue);

                // Kiểm tra luật bắt buộc rút bài nếu tổng điểm dưới 16
                if (playerTotal < 16 && !BlackJack(playerHand) && !DoubleAces(playerHand) && !FiveCardsRule(playerHand)) {
                    LowPoint = true;
                } else {
                    dealerTurn = true;
                    startTime = GetTime() - 1.5f;

                    // Nhà cái rút bài
                    while (calculateTotal(dealerHand, aiValue) < 16 && dealerHand.size() < 5 && !DoubleAces(dealerHand)) {
                        dealerHand.push_back(deck.drawCard());
                        DefaultCardBJ(dealerHand.back(), StartXPos, StartYPos);
                    }
                }
            }

            // Xử lý khi người chơi bấm nút "Restart"
            if (CheckCollisionPointRec(mousePoint, Restart)) {
                startTime = GetTime();
                currentDealerCardIndex = 0; 
                currentPlayerCardIndex = 0;
                LowPoint = false;
                resetGame(deck, playerHand, dealerHand, playerBust, dealerTurn, playerWin5CardRule, tempvalue);
            }
        }

        // Kiểm tra kết quả
        if (dealerTurn) {
            int playerTotal = calculateTotal(playerHand, tempvalue);
            int dealerTotal = calculateTotal(dealerHand, aiValue);


            if (DoubleAces(playerHand) && !DoubleAces(dealerHand)) {
                DrawText("TWO ACES! You win!", cardWidth * 2 + 50, screenHeight / 2, 30, GREEN);
            }
            else if (BlackJack(playerHand) && !BlackJack(dealerHand) && !DoubleAces(dealerHand)) {
                DrawText("BLACKJACK! You win!", cardWidth * 2 + 50, screenHeight / 2, 30, GREEN);
            }
            else if ((FiveCardsRule(playerHand) && !FiveCardsRule(dealerHand)) ||
                     ( FiveCardsRule(playerHand) && playerTotal > dealerTotal) &&
                     !DoubleAces(dealerHand) && !BlackJack(dealerHand)) {
                DrawText("You win with 5 cards under 21!", cardWidth * 2 + 50, screenHeight / 2, 30, GREEN);
            } else if (playerBust && dealerTotal <= 21) {
                DrawText("You are busted! Dealer wins!", cardWidth * 2 + 50, screenHeight / 2, 30, RED);
            } else if ((dealerTotal > 21 || playerTotal > dealerTotal) && !playerBust && !FiveCardsRule(dealerHand)) {
                DrawText("You win!", cardWidth * 2 + 50, screenHeight / 2, 30, GREEN);
            } else if ((playerTotal < dealerTotal && !playerBust) || BlackJack(dealerHand) || FiveCardsRule(dealerHand) || DoubleAces(dealerHand)) {
                DrawText("Dealer wins!", cardWidth * 2 + 50, screenHeight / 2, 30, YELLOW);
            } else {
                DrawText("It's a tie!", cardWidth * 2 + 50, screenHeight / 2, 30, ORANGE);
            }
        }
        
        DrawBackButton(clickSound);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(GetMousePosition(), BackButton)) {
                currentScreen = MENU_GAME_SCREEN;
                deck.ReturnCardsBJ(playerHand);
                deck.ReturnCardsBJ(dealerHand);
                break;
            }
        }
        EndDrawing();
    }
    currentScreen = MENU_GAME_SCREEN;
}
