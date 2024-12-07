#ifndef CARD_H
#define CARD_H

#include "raylib.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <map>
#include <random>
#include "raylib.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <map>
#include "raylib.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <map>

using namespace std;


constexpr float screenWidth = 1280;
constexpr float screenHeight = 720;

extern Texture2D cardImages;
extern Texture2D cardBack;

enum CardSuit {
    BICH,
    CHUON,
    RO,
    CO
};

// Cấu trúc để lưu trữ thông tin lá bài
struct Card {
    int rank;         // 2-10 là các lá bài số, 11-J, 12-Q, 13-K, 14-A
    int suit; // "c" - cơ, "r" - rô, "ch" - chuồn, "b" - bích
    Rectangle position;       // Vị trí hiện tại và kích thước
    Vector2 targetPosition;   // Vị trí đích
    float rotation;           // Góc xoay hiện tại
    float targetRotation;     // Góc xoay mục tiêu
    bool isMoving;            // Trạng thái đang di chuyển
    bool isFlipping;          // Trạng thái lật
    bool showBack;            // Trạng thái hiển thị mặt sau
    float flipProgress;       // Tiến trình lật (0.0 - 1.0)
    float speed;              // Tốc độ di chuyển
};


bool UpdateCardPositionAndRotation(Card &card, float x, float y);
bool UpdateCardFlip(Card &card);
void DrawCard(const Card &card);
void DefaultCard(Card &card, float x, float y);
void DefaultCardBJ(Card &card, float x, float y);

#endif // CARD_H
