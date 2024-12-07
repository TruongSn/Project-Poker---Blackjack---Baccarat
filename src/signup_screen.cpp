#include "raylib.h"
#include <cstring>
#include <string>
#include <stdio.h>
#include "src/headerFile/screens.h"
#include <src/headerFile/Menu.h>

// check coi submit chưa
using namespace std;

GameState signUpState{
    .enteringUsername = true,
    .showError = false,
    .errorMessage = "",
    .errorTimer = 0
};

// Kiểm tra xem tên người dùng đã tồn tại trong file hay chưa
bool IsUsernameExist(const char* username) {
    FILE* file = fopen("infoplayers.txt", "r");
    if (file) {
        char user[32], pass[32];
        int money, countMatch, countWin;
        float winRate;
        while (fscanf(file, "%s %s %d %d %d %f\n", user, pass, &money, &countMatch, &countWin ,&winRate) != EOF) {
            if (strcmp(user, username) == 0) {
                fclose(file);
                return true;  // Tên người dùng đã tồn tại
            }
        }
        fclose(file);
    }
    return false;  // Tên người dùng chưa tồn tại
}

void ShowError(const char* message) {
    signUpState.showError = true;
    signUpState.errorMessage = message;
    signUpState.errorTimer = 2.0f;
}

void ClearUserDataFields() {
    memset(signUpState.userData.username, 0, sizeof(signUpState.userData.username));
    memset(signUpState.userData.password, 0, sizeof(signUpState.userData.password));
}

void SaveUserDataToFile(const UserData& userData) {
    FILE *file = fopen("infoplayers.txt", "a");
    if (file != NULL) {
        // Lưu username, password, money, countMatch, countWin, và winRate vào file
        fprintf(file, "%s %s %d %d %d %.2f\n", userData.username, userData.password, userData.money, userData.countMatch, userData.countWin, userData.winRate);
        fclose(file);
    } else {
        ShowError("Could not save user data!");
    }
}

void UpdateSignupScreen() {
    if (signUpState.showError) {
        signUpState.errorTimer -= GetFrameTime();
        if (signUpState.errorTimer <= 0.0f) {
            signUpState.showError = false;
        }
    }

    // Kiểm tra phím TAB để chuyển giữa Username và Password
    if (IsKeyPressed(KEY_TAB)) {
        signUpState.enteringUsername = !signUpState.enteringUsername;
    }

    // Cập nhật input cho username
    if (signUpState.enteringUsername) {
        // Nếu nhấn một phím bất kỳ ngoài phím Backspace thì thêm vào tên đăng nhập
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (strlen(signUpState.userData.username) > 0) {
                signUpState.userData.username[strlen(signUpState.userData.username) - 1] = '\0';  // Xóa ký tự cuối cùng
            }
        } else {
            for (int key = 32; key < 128; key++) {  // Kiểm tra các phím ký tự
                if (IsKeyPressed(key)) {
                    char keyChar = (char)key;
                    // Chỉ cho phép nhập khi tên người dùng chưa vượt quá chiều dài cho phép
                    if (strlen(signUpState.userData.username) < sizeof(signUpState.userData.username) - 1) {
                        int length = strlen(signUpState.userData.username);
                        signUpState.userData.username[length] = keyChar;  // Thêm ký tự mới
                        signUpState.userData.username[length + 1] = '\0';  // Kết thúc chuỗi
                    }
                }
            }
        }
    }

    // Cập nhật input cho password
    if (!signUpState.enteringUsername) {
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (strlen(signUpState.userData.password) > 0) {
                signUpState.userData.password[strlen(signUpState.userData.password) - 1] = '\0';  // Xóa ký tự cuối cùng
            }
        } else {
            for (int key = 32; key < 128; key++) {  // Kiểm tra các phím ký tự
                if (IsKeyPressed(key)) {
                    char keyChar = (char)key;
                    // Chỉ cho phép nhập khi mật khẩu chưa vượt quá chiều dài cho phép
                    if (strlen(signUpState.userData.password) < sizeof(signUpState.userData.password) - 1) {
                        int length = strlen(signUpState.userData.password);
                        signUpState.userData.password[length] = keyChar;  // Thêm ký tự mới
                        signUpState.userData.password[length + 1] = '\0';  // Kết thúc chuỗi
                    }
                }
            }
        }
    }
}


void DrawSignupScreen() {
    ClearBackground(RAYWHITE);
    DrawTexturePro(menuBackground, 
        Rectangle{0, 0, (float)menuBackground.width, (float)menuBackground.height}, 
        Rectangle{0, 0, screenWidth, screenHeight}, 
        Vector2{0, 0}, 
        0, 
        WHITE);

    // Tiêu đề "SIGN UP SCREEN"
    const char* title = "SIGN UP SCREEN";
    int titleFontSize = 50; // Font lớn hơn
    int textWidth = MeasureText(title, titleFontSize);
    int x = (GetScreenWidth() - textWidth) / 2;
    float fade = 0.5f;
    float fade2 = 0.3f;

    Rectangle titleRect = {x - 10, 75, textWidth + 20, titleFontSize + 10};
    DrawRectangleRec(titleRect, Fade(DARKBLUE, fade));
    DrawText(title, x, 80, titleFontSize, GOLD);  // Màu vàng nổi bật
    DrawRectangleLinesEx(titleRect, 3, Fade(GOLD, fade));  // Khung trong suốt cho tiêu đề

    DrawRectangleRounded(Rectangle{80, 170, screenWidth - 160, 310}, 0.2f, 1, Fade(BLACK, fade));

    Color usernameColor = signUpState.enteringUsername ? SKYBLUE : LIGHTGRAY;
    Color passwordColor = !signUpState.enteringUsername ? SKYBLUE : LIGHTGRAY;

    // Vị trí và kích thước các trường nhập liệu
    Rectangle usernameRect = {95, 195, (float)(MeasureText(TextFormat("Username: %s", signUpState.userData.username), 30)) + 10, 40};
    Rectangle passwordRect = {95, 265, (float)(MeasureText(TextFormat("Password: %s", string(strlen(signUpState.userData.password), '*').c_str()), 30)) + 10, 40};

    DrawRectangleRec(usernameRect, Fade(BLACK, fade2));
    DrawRectangleRec(passwordRect, Fade(BLACK, fade2));

    // Vẽ phần nhập username
    DrawText(TextFormat("Username: %s", signUpState.userData.username), 100, 200, 30, usernameColor);
    DrawRectangleLinesEx(usernameRect, 3, Fade(SKYBLUE, fade));

    // Vẽ phần nhập password
    DrawText(TextFormat("Password: %s", string(strlen(signUpState.userData.password), '*').c_str()), 100, 270, 30, passwordColor);
    DrawRectangleLinesEx(passwordRect, 3, Fade(SKYBLUE, fade));

    // Nút "Sign Up"
    Rectangle signUpButton = {screenWidth / 2 - 100, 330, 200, 60};
    bool isButtonHovered = CheckCollisionPointRec(GetMousePosition(), signUpButton);
    Color buttonColor = isButtonHovered ? Fade(DARKBLUE, 0.5f) : Fade(BLUE, fade);
    Color textColor = isButtonHovered ? Fade(GOLD, fade) : RAYWHITE;
    DrawRectangleRec(signUpButton, buttonColor);
    DrawText("Sign Up", signUpButton.x + 35, signUpButton.y + 15, 30, textColor);
    DrawRectangleLinesEx(signUpButton, 3, Fade(GOLD, fade));

    // Nút "Back"
    Rectangle backButton = {10, 10, 100, 40};
    bool isBackButtonHovered = CheckCollisionPointRec(GetMousePosition(), backButton);
    Color backButtonColor = isBackButtonHovered ? Fade(DARKBLUE, 0.5f) : Fade(LIGHTGRAY, fade);
    Color backTextColor = isBackButtonHovered ? RAYWHITE : BLACK;
    DrawRectangleRec(backButton, backButtonColor);
    DrawText("Back", backButton.x + 20, backButton.y + 10, 20, backTextColor);
    DrawRectangleLinesEx(backButton, 3, Fade(BLUE, fade));

    // Xử lý sự kiện khi nhấn nút Sign Up
    bool trySignUp = IsKeyPressed(KEY_ENTER) ||
                     (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                      CheckCollisionPointRec(GetMousePosition(), signUpButton));

    if (trySignUp) {
        if (strlen(signUpState.userData.username) < 5) {
            ShowError("Username must be at least 5 characters!");
        } else if (strlen(signUpState.userData.password) < 5) {
            ShowError("Password must be at least 5 characters!");
        } else if (IsUsernameExist(signUpState.userData.username)) {
            ShowError("Username already exists!");
        } else {
            ShowError("Sign-up successful!");
            // Khởi tạo tiền và tỷ lệ thắng cho người chơi khi đăng ký
            signUpState.userData.money = 1000;  // Tiền khởi tạo
            signUpState.userData.countMatch= 0;
            signUpState.userData.countWin = 0;
            signUpState.userData.winRate = 0.0f; // Tỷ lệ thắng khởi tạo là 0

            SaveUserDataToFile(signUpState.userData); // Lưu thông tin vào file
            ClearUserDataFields();  // Xóa thông tin người dùng
        }
    }



    // Xử lý nút "Back"
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), backButton)) {
            currentScreen = SIGNIN_SCREEN;  // Quay lại màn hình đăng nhập
        }
    }

    if (signUpState.showError) {
        Rectangle failedRect = {screenWidth / 2 - MeasureText(signUpState.errorMessage, 30) / 2 - 10,
                                 420, MeasureText(signUpState.errorMessage, 30) + 20, 50};
        DrawRectangleRec(failedRect, Fade(DARKBLUE, fade2));
        DrawText(signUpState.errorMessage, screenWidth / 2 - MeasureText(signUpState.errorMessage, 30) / 2, 430, 30, RED);
        DrawRectangleLinesEx(failedRect, 3, Fade(RED, fade2));
    }

    // Hướng dẫn sử dụng phím TAB
    const char* introduction = "Press TAB to switch between Username and Password";
    Rectangle tabButton = {100, 490, MeasureText(introduction, 20) + 30, 40};
    DrawRectangleRec(tabButton, Fade(DARKBROWN, fade2));
    DrawText(introduction, 100 + 15, 490 + 10, 20, WHITE);
    DrawRectangleLinesEx(tabButton, 2, Fade(WHITE, fade2));
}

