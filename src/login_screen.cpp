#include "raylib.h"
#include "src/headerFile/Menu.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring> 
#include <algorithm> // Thêm thư viện để sử dụng transform

UserData userData; // Khai báo đúng kiểu UserData

using namespace std;


GameState signInState{
    .enteringUsername = true,
    .showError = false,
    .errorMessage = "",
    .errorTimer = 0
};

void ShowErrorLogin(const char* message) {
    signInState.showError = true;
    signInState.errorMessage = message;
    signInState.errorTimer = 2.0f;
}

extern GameScreen currentScreen;  

// Hàm kiểm tra thông tin đăng nhập từ file text
bool CheckLogin(const string& inputUsername, const string& inputPassword) {
    ifstream file("infoplayers.txt"); // Đảm bảo tên file là chính xác
    if (!file.is_open()) {
        cout << "Could not open file!" << endl;
        return false;
    }

    string fileUsername, filePassword;
    int fileMoney, fileCountMatch, fileCountWin;
    float fileWinRate;
    while (file >> fileUsername >> filePassword >> fileMoney >> fileCountMatch >> fileCountWin >>fileWinRate) {
        // Chuyển đổi thành chữ thường
        string lowerFileUsername = fileUsername;
        string lowerFilePassword = filePassword;
        string lowerInputUsername = inputUsername;
        string lowerInputPassword = inputPassword;

        transform(lowerFileUsername.begin(), lowerFileUsername.end(), lowerFileUsername.begin(), ::tolower);
        transform(lowerFilePassword.begin(), lowerFilePassword.end(), lowerFilePassword.begin(), ::tolower);
        transform(lowerInputUsername.begin(), lowerInputUsername.end(), lowerInputUsername.begin(), ::tolower);
        transform(lowerInputPassword.begin(), lowerInputPassword.end(), lowerInputPassword.begin(), ::tolower);


        if (lowerInputUsername == lowerFileUsername && lowerInputPassword == lowerFilePassword) {
            // Sử dụng strcpy để sao chép vào mảng char[]
            strcpy(userData.username, lowerInputUsername.c_str());  // Sao chép username
            strcpy(userData.password, lowerFilePassword.c_str());  // Sao chép password
            userData.money = fileMoney;
            userData.countMatch = fileCountMatch;
            userData.countWin = fileCountWin;
            userData.winRate = fileWinRate;
            file.close();
            return true;  // Đăng nhập thành công
        }
    }

    file.close();
    return false;  
}

// Hàm cập nhật logic màn hình login
void UpdateLoginScreen() {
    if (signInState.showError) {
        signInState.errorTimer -= GetFrameTime();
        if (signInState.errorTimer <= 0) {
            signInState.showError = false;
        }
    }

    if (IsKeyPressed(KEY_TAB)) {
        signInState.enteringUsername = !signInState.enteringUsername;
    }

    int key = GetKeyPressed();
    char* currentField = signInState.enteringUsername ? signInState.userData.username : signInState.userData.password;

    if (key >= 32 && key <= 126 && strlen(currentField) < MAX_INPUT_LENGTH) {
        currentField[strlen(currentField)] = (char)key;
        currentField[strlen(currentField) + 1] = '\0';
    }

    if (IsKeyPressed(KEY_BACKSPACE) && strlen(currentField) > 0) {
        currentField[strlen(currentField) - 1] = '\0';
    }
}



// Hàm vẽ màn hình login

void DrawLoginScreen() {
    ClearBackground(BLACK);  // Nền đen
    DrawTexturePro(menuBackground, 
        Rectangle{0, 0, (float)menuBackground.width, (float)menuBackground.height}, 
        Rectangle{0, 0, screenWidth, screenHeight}, 
        Vector2{0, 0}, 
        0, 
        WHITE);

    // Tiêu đề "LOGIN SCREEN"
    const char* title = "LOGIN SCREEN";
    int titleFontSize = 50; // Font lớn hơn
    int textWidth = MeasureText(title, titleFontSize);
    int x = (GetScreenWidth() - textWidth) / 2;
    float fade = 0.5f;
    float fade2 = 0.3f;


    Rectangle titleRect = {x - 10, 75, textWidth + 20, titleFontSize + 10};
    DrawRectangleRec(titleRect, Fade(DARKBLUE, fade));
    DrawText(title, x, 80, titleFontSize, GOLD);  // Màu vàng nổi bật
    DrawRectangleLinesEx(titleRect, 3, Fade(GOLD, fade));  // Khung trong suốt cho tiêu đề

    DrawRectangleRounded(Rectangle{80, 170, screenWidth - 160, 390}, 0.2f, 1, Fade(BLACK, fade));

    // Vị trí và kích thước các trường nhập liệu
    Rectangle usernameRect = {95 + 30, 195, (float)(MeasureText(TextFormat("Username: %s", signInState.userData.username), 30)) + 10, 40};
    Rectangle passwordRect = {95 + 30, 265, (float)(MeasureText(TextFormat("Password: %s", string(strlen(signInState.userData.password), '*').c_str()), 30)) + 10, 40};
    DrawRectangleRec(usernameRect, Fade(BLACK, fade2));
    DrawRectangleRec(passwordRect, Fade(BLACK, fade2));


    // Vẽ phần nhập signInState.userData.username
    Color usernameColor = signInState.enteringUsername ? SKYBLUE : LIGHTGRAY;
    DrawText(TextFormat("Username: %s", signInState.userData.username), 100 + 30, 200, 30, usernameColor);
    DrawRectangleLinesEx(usernameRect, 3, Fade(SKYBLUE, fade));

    // Vẽ phần nhập signInState.userData.password
    Color passwordColor = !signInState.enteringUsername ? SKYBLUE : LIGHTGRAY;
    DrawText(TextFormat("Password: %s", string(strlen(signInState.userData.password), '*').c_str()), 100 + 30, 270, 30, passwordColor);
    DrawRectangleLinesEx(passwordRect, 3, Fade(SKYBLUE, fade));

    // Nút "Sign Up" 
    Rectangle signUpButton = {100, 420, 200, 60};
    bool isSignUpButtonHovered = CheckCollisionPointRec(GetMousePosition(), signUpButton);
    Color signUpButtonColor = isSignUpButtonHovered ? Fade(DARKBLUE, 0.5f) : Fade(BLUE, fade);
    Color signUpTextColor = isSignUpButtonHovered ? Fade(GOLD, fade) : RAYWHITE;
    DrawRectangleRec(signUpButton, Fade(signUpButtonColor, fade));
    DrawText("Sign Up?", signUpButton.x + 35, signUpButton.y + 15, 30, signUpTextColor);
    DrawRectangleLinesEx(signUpButton, 3, Fade(GOLD, fade));

    // Nút "Login"
    Rectangle loginButton = {100, 330, 200, 60};
    bool isLoginButtonHovered = CheckCollisionPointRec(GetMousePosition(), loginButton);
    Color loginButtonColor = isLoginButtonHovered ? Fade(DARKBLUE, 0.5f) : Fade(BLUE, fade);
    Color loginTextColor = isLoginButtonHovered ? Fade(GOLD, fade) : RAYWHITE;
    DrawRectangleRec(loginButton, Fade(loginButtonColor, fade));
    DrawText("Login", loginButton.x + (loginButton.width / 2 - MeasureText("Login", 30) / 2), loginButton.y + 15, 30, loginTextColor);
    DrawRectangleLinesEx(loginButton, 3, Fade(GOLD, fade));

    // Hướng dẫn sử dụng phím TAB
    const char* introduction = "Press TAB to switch between Username and Password";
    Rectangle tabButton = {100, 580, MeasureText(introduction, 20) + 30, 40};
    DrawRectangleRec(tabButton, Fade(DARKBROWN, fade2));
    DrawText(introduction, 100 + 15, 580 + 10, 20, LIGHTGRAY);

    // Chuyển sang màn hình đăng ký khi bấm nút Sign Up
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), signUpButton)) {
            currentScreen = SIGNUP_SCREEN;
        }
    }

    // Xử lý nút Login
    if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), loginButton))
            || IsKeyPressed(KEY_ENTER)) {
        if (CheckLogin(signInState.userData.username, signInState.userData.password)) {
            signInState.showError = false;
            currentScreen = MENU_GAME_SCREEN;  // Chuyển đến màn hình chính
        } else {
            ShowErrorLogin("Login failed! Incorrect Username or Password.");  // Đăng nhập thất bại        }
        }
    }


    // Thông báo khi đăng nhập thất bại
    if (signInState.showError) {
        Rectangle failedRect = {screenWidth / 2 - MeasureText(signInState.errorMessage, 30) / 2 - 10, 535 - 30, (float)(MeasureText(signInState.errorMessage, 30)) + 20, 40};
        DrawRectangleRec(failedRect, Fade(DARKBLUE, fade2));
        DrawText(signInState.errorMessage, screenWidth / 2 - MeasureText(signInState.errorMessage, 30) / 2, 540 - 30, 30, RED);
        DrawRectangleLinesEx(failedRect, 3, Fade(RED, fade));
    }
}
