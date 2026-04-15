#pragma once
#include "State.h"
#include "Button.h"
#include <vector>

// Cấu trúc mô phỏng một chiếc lá
struct Leaf {
    sf::ConvexShape shape;
    float speedY;         // Tốc độ rơi xuống
    float baseSpeedX;     // Hướng gió thổi ngang cơ bản
    float rotationSpeed;  // Tốc độ xoay của lá
    float timeOffset;     // Dùng cho hàm sin() để tạo độ lượn sóng
};

class MainMenu : public State {
private:
    Button* btnSLL;
    Button* btnAVL;
    Button* btnHeap;
    Button* btnAA;
    Button* btnMST;
    Button* btnDijkstra;
    sf::Text titleText; // Thêm biến này để làm tiêu đề
    sf::Text titleShadow; // Thêm biến bóng đổ cho tiêu đề
    // ... các biến khác giữ nguyên
    // Quản lý danh sách lá
    std::vector<Leaf> leaves;
    void initLeaves(int count);

public:
    MainMenu(App* app);
    ~MainMenu();

    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
};