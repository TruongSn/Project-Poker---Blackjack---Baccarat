#include "src/headerFile/Deck.h"


// Constructor
Deck::Deck() {
    createDeck();
    shuffleDeck();
}

// Destructor
Deck::~Deck() {
    cards.clear(); // Không cần delete thủ công vì vector tự quản lý bộ nhớ
}

// Tạo bộ bài
void Deck::createDeck() {
    cards.clear();
    for (int i = 2; i <= 14; i++) {
        for (const int suit : {BICH, CHUON, RO, CO}) {            
            Card newCard;
            newCard.rank = i;
            newCard.suit = suit;
            newCard.position = {float(GetScreenHeight())/2, float(GetScreenWidth())/2, 180, 270};  // Kích thước mặc định
            newCard.targetPosition = {0, 0};  // Vị trí đích ban đầu
            newCard.rotation = -300.0f;
            newCard.targetRotation = 360.0f;
            newCard.isMoving = false;
            newCard.isFlipping = false;
            newCard.showBack = false;
            newCard.flipProgress = 0.0f;
            newCard.speed = 10.0f;  // Tốc độ mặc định
            cards.push_back(newCard);
        }
    }
}

// Hiển thị hình ảnh lá bài
void Deck::showCardImage(Card& card) {
    // Nếu đang lật, vẽ mặt sau hoặc mặt trước tùy theo trạng thái
    DrawCard(card);
}

void Deck::UpdateCards(vector<Card> &cards, 
                 float y, 
                 int &currentCardIndex,
                 int cardSpacing, 
                 int aimXPos) {
    if (currentCardIndex < cards.size()) {
        cards[currentCardIndex].isMoving = true;
        bool reachedTarget = UpdateCardPositionAndRotation(
            cards[currentCardIndex], 
            aimXPos + currentCardIndex * (180 + cardSpacing), 
            y
        );

        if (reachedTarget) {
            currentCardIndex++; // Chuyển sang lá bài tiếp theo
        }
    }
}


// Cập nhật trạng thái lá bài
void Deck::updateCard1(Card& card, float x, float y) {
    card.isMoving = true;
    if (UpdateCardPositionAndRotation(card, x, y)) {
        if (!card.isFlipping) {
            card.isFlipping = true;
        } 
        else {
            UpdateCardFlip(card);
            }
        }
}
void Deck::updateCard2(Card& card, float x, float y) {
    card.isMoving = true;
    UpdateCardPositionAndRotation(card, x, y);
}

void Deck::revealCard(Card& card) {
    if (!card.isMoving){
        card.isFlipping = true;
        UpdateCardFlip(card);
        DrawCard(card);
    }
}


// Trộn bộ bài
void Deck::shuffleDeck() {
    shuffle(cards.begin(), cards.end(), mt19937(random_device()()));
}

// Rút lá bài từ bộ bài
Card Deck::drawCard() {
    if (cards.empty()) {
        createDeck();
        shuffleDeck();
    }
    Card card = cards.back();
    cards.pop_back();
    return card;
}

void Deck::DrawAllCards(const vector<Card> &cards) {
    for (const auto &card : cards) {
        DrawCard(card);
    }
}

void Deck::ReturnCardsBJ(vector<Card> &hand) {
    cards.insert(cards.end(), make_move_iterator(hand.begin()), make_move_iterator(hand.end()));
    hand.clear();
}

void Deck::ReturnCards(Card* hand) {
    for (int i = 0; i < 5; i++) {
        cards.push_back(hand[i]);
    }
}