#pragma once
#include "State.h"
#include "Button.h"
#include "InputBox.h"
#include "SinglyLinkedList.h"
#include "PlaybackController.h"

class App;

class SLLScreen : public State {
private:
    Button* btnBackToMenu;
    Button* btnInsert;
    Button* btnSearch;
    Button* btnDelete;
    Button* btnInit;

    InputBox* inputVal;
    SinglyLinkedList* sll;
    PlaybackController* playbackController;

    // --- THÊM 4 BIẾN NÀY CHO TÍNH NĂNG CAMERA ---
    sf::View listView;           // Camera để soi danh sách
    bool isPanning = false;      // Đang giữ chuột để kéo hay không?
    sf::Vector2i oldMousePos;    // Lưu vị trí chuột cũ để tính khoảng cách kéo
    bool isViewInitialized = false; // Đảm bảo Camera chỉ khởi tạo 1 lần

public:
    // ... (Giữ nguyên phần public)
    SLLScreen(App* app);
    ~SLLScreen();

    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
};