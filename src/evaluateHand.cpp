#include "src/headerFile/evaluateHand.h"



// Xếp hạng của tay bài
enum HandRank {
    HIGH_CARD = 1,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH,
    ROYAL_FLUSH
};


// Hàm đánh giá hạng của tay bài
pair<int, vector<int>> evaluateHand(const Card* hand) {

    map<int, int> rankCount;
    map<int, int> suitCount;

    for (size_t i = 0; i < 5; i++) {
        rankCount[hand[i].rank]++;
        suitCount[hand[i].suit + 1]++;
    }

    vector<int> ranks;
    for (const auto& pair : rankCount) ranks.push_back(pair.first);
    sort(ranks.begin(), ranks.end(), greater<int>());

    bool isFlush = (suitCount.size() == 1);
    bool isStraight = false;
    if (ranks.size() == 5 && ranks[0] - ranks[4] == 4) {
        isStraight = true;
    } else if (ranks == vector<int>{14, 5, 4, 3, 2}) {
        isStraight = true;
        ranks = {5, 4, 3, 2, 1};
    }

    if (isFlush && isStraight && ranks[0] == 14) return {ROYAL_FLUSH, ranks};
    if (isFlush && isStraight) return {STRAIGHT_FLUSH, ranks};
    for (const auto& pair : rankCount) {
        if (pair.second == 4) return {FOUR_OF_A_KIND, {pair.first}};
    }
    if (rankCount.size() == 2) {
        for (const auto& pair : rankCount) {
            if (pair.second == 3) return {FULL_HOUSE, {pair.first}};
        }
    }
    if (isFlush) return {FLUSH, ranks};
    if (isStraight) return {STRAIGHT, ranks};
    for (const auto& pair : rankCount) {
        if (pair.second == 3) return {THREE_OF_A_KIND, {pair.first}};
    }
    if (rankCount.size() == 3) {
        vector<int> pairs;
        for (const auto& pair : rankCount) {
            if (pair.second == 2) pairs.push_back(pair.first);
        }
        sort(pairs.rbegin(), pairs.rend());
        return {TWO_PAIR, pairs};
    }
    if (rankCount.size() == 4) {
        for (const auto& pair : rankCount) {
            if (pair.second == 2) return {ONE_PAIR, {pair.first}};
        }
    }
    return {HIGH_CARD, ranks};
}

// Hàm so sánh hai tay bài
string compareHands(const Card* playerHand, const Card* aiHand) {
    auto playerResult = evaluateHand(playerHand);
    auto aiResult = evaluateHand(aiHand);

    if (playerResult.first > aiResult.first) return "Player wins!";
    if (playerResult.first < aiResult.first) return "AI wins!";
    for (size_t i = 0; i < playerResult.second.size(); ++i) {
        if (playerResult.second[i] > aiResult.second[i]) return "Player wins!";
        if (playerResult.second[i] < aiResult.second[i]) return "AI wins!";
    }
    return "It's a tie!";
}