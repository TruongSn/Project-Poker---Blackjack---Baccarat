#include "src/headerFile/Menu.h"


// Hàm vẽ thông tin chi tiết người chơi
void DrawPlayerDetails(const UserData& user, float x, float y) {
    DrawRectangle(x, y, 300, 300, Fade(BLUE, 0.5f)); // Nền bảng bên trái
    DrawRectangleLines(x, y, 300, 300, WHITE);
    DrawText("Player Details", x + 20, y + 10, 24, WHITE);

    DrawText(("Username: ", user.username), x + 20, y + 60, 20, WHITE);
    DrawText(("Money: " + to_string(user.money)).c_str(), x + 20, y + 100, 20, WHITE);
    DrawText(("Matches: " + to_string(user.countMatch)).c_str(), x + 20, y + 140, 20, WHITE);
    DrawText(("Wins: " + to_string(user.countWin)).c_str(), x + 20, y + 180, 20, WHITE);

    // Chỉnh sửa cách sử dụng TextFormat
    string formatedWinRate = TextFormat("Win Rate: %.2f%%", user.winRate * 100);

    // Vẽ thông tin Win Rate
    DrawText(formatedWinRate.c_str(), x + 20, y + 220, 20, WHITE);

}


int currentPage = 0;  // Số trang hiện tại
const int usersPerPage = 10;  // Số người chơi mỗi trang

void leaderboard() {
    ifstream file("infoplayers.txt");
    if (!file.is_open()) {
        cout << "Không thể mở file infoplayers.txt!" << endl;
        return;
    }

    vector<UserData> users; // Vector lưu thông tin người dùng
    string line;

    // Đọc thông tin từ file
    while (getline(file, line)) {
        istringstream iss(line);
        UserData user;
        if (iss >> user.username >> user.password >> user.money >> user.countMatch >> user.countWin >> user.winRate) {
            users.push_back(user); // Lưu thông tin vào vector
        }
    }
    file.close();

    // Sắp xếp vector theo winRate giảm dần
    sort(users.begin(), users.end(), [](const UserData& a, const UserData& b) {
        return a.winRate > b.winRate;
    });

    // Tổng số trang
    int totalPages = (users.size() + usersPerPage - 1) / usersPerPage; 
    DrawLeaderboard(users, currentPage, totalPages);
}

// Hàm vẽ bảng xếp hạng cho trang hiện tại
void DrawLeaderboard(const vector<UserData>& users, int currentPage, int totalPages) {

    // Biến lưu trữ người chơi được chọn
    int selectedIndex = -1;

    while (!WindowShouldClose()) {
    Vector2 mousePosition = GetMousePosition();
    
    int startIdx = currentPage * usersPerPage;
    int endIdx = min(startIdx + usersPerPage, (int)users.size());

    // Xử lý chọn người chơi khi nhấp chuột
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        float rowHeight = 50;
        for (int i = startIdx; i < endIdx; i++) {
            float rowY = 100 + (i - 10 * currentPage + 1) * rowHeight;
            if (mousePosition.y > rowY && mousePosition.y < rowY + rowHeight) {
                selectedIndex = i; // Lưu chỉ số hàng được chọn
                break;
            }
        }
    }
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexturePro(LBBackground, {float(LBBackground.width) / 4, 0, float(LBBackground.width) * 3 / 4, float(LBBackground.height)}, {0, 0, screenWidth, screenHeight}, {0, 0}, 0, WHITE);

    

    float rowHeight = 50; // Chiều cao mỗi hàng
    float colRankWidth = 80;
    float colNameWidth = 420;
    float colWinRate = 200;
    float x = 450.0f, y = 100.0f;


    DrawRectangleRoundedLinesEx(Rectangle{800.0f - MeasureText("Leaderboard", 48) / 2 -10, 14, float(MeasureText("Leaderboard", 48)) + 30, 60}, 0.2f, 4, 5, BLUE);
    // Vẽ tiêu đề
    DrawTextEx(titleFont, "Leaderboard", {800.0f - MeasureText("Leaderboard", 48) / 2 + 5, 24}, 48, 2, RAYWHITE);

    // Vẽ header
    DrawRectangle(x, y, colRankWidth + colNameWidth + colWinRate, rowHeight, Fade(DARKBLUE, 0.8f));
    DrawRectangleLines(x, y, colRankWidth + colNameWidth + colWinRate, rowHeight, WHITE);
    DrawText("Rank", x + 10, y + 15, 20, WHITE);
    DrawText("Name", x + colRankWidth + 10, y + 15, 20, WHITE);
    DrawText("Win Rate", x + colRankWidth + colNameWidth + 10, y + 15, 20, WHITE);

    // Vẽ các hàng
    for (int i = startIdx; i < endIdx; i++) {
        float rowY = y + (i - 10 * currentPage + 1) * rowHeight;

        Color rowColor = (i == selectedIndex) ? DARKBLUE : Fade(BLUE, 0.6f);
        DrawRectangle(x, rowY, colRankWidth + colNameWidth + colWinRate, rowHeight, rowColor);

        string formattedWinRate = string(TextFormat("%.2f", users[i].winRate * 100)) + "%";

        DrawText(to_string(i + 1).c_str(), x + 10, rowY + 15, 20, WHITE);
        DrawText(users[i].username, x + colRankWidth + 10, rowY + 15, 20, WHITE);
        DrawText(formattedWinRate.c_str(), x + colRankWidth + colNameWidth + 10, rowY + 15, 20, WHITE);
    }

    // Vẽ viền bảng
    DrawRectangleLines(x, y, colRankWidth, (endIdx - 10 * currentPage) * rowHeight + 50, WHITE);
    DrawRectangleLines(x + colRankWidth, y, colNameWidth, (endIdx - 10 * currentPage) * rowHeight + 50, WHITE);
    DrawRectangleLines(x + colRankWidth + colNameWidth, y, colWinRate, (endIdx - 10 * currentPage) * rowHeight + 50, WHITE);

    if (selectedIndex != -1) {
        DrawPlayerDetails(users[selectedIndex], 50, 150); // Vẽ thông tin chi tiết
    }

    
    // Vẽ trang thông tin
    string pageInfo = TextFormat("Page %d of %d", currentPage + 1, totalPages);
    DrawText(pageInfo.c_str(), 800 - MeasureText(pageInfo.c_str(), 20) / 2,  y + (endIdx + 1 - 10 * currentPage) * rowHeight + 30, 20, RAYWHITE);
    // vẽ nút Next và Previous 
    if (ButtonT("Previous", 800 - 120 - MeasureText(pageInfo.c_str(), 20) / 2 - 10, y + (endIdx + 1 - 10 * currentPage) * rowHeight + 20, 120, 40)) {
        if (currentPage > 0) {
            currentPage--;
            selectedIndex = -1;
        }
    }
    if (ButtonT("Next", 800 + MeasureText(pageInfo.c_str(), 20) / 2 + 10, y + (endIdx + 1 - 10 * currentPage) * rowHeight + 20, 120, 40)) {
        if (currentPage < totalPages - 1) {
            selectedIndex = -1;
            currentPage++;
        }
    }

    DrawBackButton(clickSound);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), BackButton)) {
            currentScreen = MENU_GAME_SCREEN;
            break;
        }
    }

    EndDrawing();
    }
}


// Hàm nút giả
bool ButtonT(const char* text, int x, int y, int width, int height) {
    // Vẽ nền nút
    DrawRectangle(x, y, width, height, DARKBLUE);
    DrawRectangleLines(x, y, width, height, WHITE);

    //Vẽ văn bản trên nút
    DrawText(text, x + 10, y + 10, 20, WHITE);

    // Trả về true nếu nút được nhấn
    return CheckCollisionPointRec(GetMousePosition(), Rectangle{(float)x, (float)y, (float)width, (float)height}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}