#ifndef DECK_H
#define DECK_H

#include <src/headerFile/Card.h>


// Cấu trúc để lưu trữ thông tin của bộ bài
class Deck {
private:
    void createDeck();  // Tạo bộ bài
    void unloadCardImages(Card& card);  // Giải phóng tài nguyên hình ảnh

public:
    vector<Card> cards;  // Danh sách các lá bài


    Deck();                   // Constructor
    ~Deck();                  // Destructor


    void UpdateCards(vector<Card> &cards, 
                 float y, 
                 int &currentCardIndex,
                 int cardSpacing, 
                 int aimXPos);
    void updateCard1(Card& card, float x, float y);   // Cập nhật trạng thái lá bài
    void updateCard2(Card& card, float x, float y);    
    void shuffleDeck();       // Trộn bộ bài
    void revealCard(Card& card); // Lật bài
    Card drawCard();          // Rút lá bài từ bộ bài
    void showCardImage(Card& card); // Hiển thị hình ảnh lá bài
    void DrawAllCards(const vector<Card> &cards);
    void ReturnCardsBJ(vector<Card> &hand);
    void ReturnCards(Card* hand);
};



#endif // DECK_H
