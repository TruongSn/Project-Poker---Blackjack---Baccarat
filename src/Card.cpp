#include <src/headerFile/Card.h>

// origin witdh
float oriCardwidth = 360;
// origin height
float oriCardheight = 540;


void DefaultCard(Card &card, float x, float y){
    card.position = {x, y, 180, 270}; 
    card.flipProgress = 0.0f;
    card.rotation = -400.0f;
    card.speed = 10.0f;
}

void DefaultCardBJ(Card &card, float x, float y){
    card.position = {x, y, 180, 270}; 
    card.flipProgress = 0.0f;
    card.rotation = 0.0f;
    card.speed = 22.0f;
}



bool UpdateCardPositionAndRotation(Card &card, float x, float y) {
    bool reachedTarget = true;

    // Tính toán đường đi đến vị trí
    if (card.isMoving) {
        float dx = x - card.position.x;
        float dy = y - card.position.y;

        float distance = sqrt(dx * dx + dy * dy);
        if (distance > card.speed) {
            card.position.x += (dx / distance) * card.speed;
            card.position.y += (dy / distance) * card.speed;
            reachedTarget = false;
        } else {
            card.position.x = x;
            card.position.y = y;
        }

        
        // Tính toán xoay 1 vòng
        float rotationDiff = card.targetRotation - card.rotation;
        if (fabs(rotationDiff) > 5.0f) {
            // Tăng tốc độ xoay khi rotationDiff nhỏ hơn
            float speedFactor = 0.03f + 5.0f / (fabs(rotationDiff) + 3.0f); // Tăng khi gần
            card.rotation += rotationDiff * speedFactor;
            reachedTarget = false;
        } else {
            card.rotation = card.targetRotation;
        }


        // Đã đến vị trí
        if (reachedTarget) {
            card.isMoving = false;
        }
    }

    return reachedTarget;
}

bool UpdateCardFlip(Card &card) {
    if (card.isFlipping) {
        card.flipProgress += 0.05f;

        if (card.flipProgress >= 1.0f) {
            card.flipProgress = 1.0f; // Reset trạng thái lật
            card.isFlipping = false;
            card.showBack = !card.showBack; // Đổi trạng thái hiển thị
        }

        return false; // Vẫn đang lật
    }

    return true; // Đã lật xong
}

void DrawCard(const Card &card) {

    Texture2D texture = (card.flipProgress < 0.5f) ? cardBack : cardImages;
    float scaleX = (card.flipProgress < 0.5f) ? 1.0f - 2.0f * card.flipProgress : -1.0f + 2.0f * card.flipProgress;
    float cardWidth = card.position.width * scaleX;
    float cardHeight = card.position.height;

    float posX = card.position.x + cardWidth/2;
    float posY = card.position.y + cardHeight/2;

    // tính toán lá bài đang vẽ
    int num = card.rank - 2;
    int suit = card.suit;


    Vector2 origin = {card.position.width / 2, card.position.height / 2};
    DrawTexturePro(
        texture,
        Rectangle{oriCardwidth * suit, oriCardheight * num, oriCardwidth, oriCardheight},
        Rectangle{posX, posY, cardWidth, cardHeight},
        origin,
        card.rotation,
        WHITE
    );
}