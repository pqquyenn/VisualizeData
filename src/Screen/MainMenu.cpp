#include "MainMenu.h"
#include "App.h"
#include "SLLScreen.h"
#include "AVLScreen.h"
#include "HeapScreen.h"
#include "AAScreen.h"
#include "MSTScreen.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

MainMenu::MainMenu(App* app) : State(app) {
    // Khởi tạo seed cho hàm random
    srand(static_cast<unsigned int>(time(NULL)));

    // Tạo các nút bấm (đã sửa lại tọa độ Y của MST thành 700 để không đè lên AA)
    btnSLL = new Button(540, 300, 200, 50, app->font, "Singly Linked List");
    btnAVL = new Button(540, 400, 200, 50, app->font, "AVL tree");
    btnHeap = new Button(540, 500, 200, 50, app->font, "Max Heap");
    btnAA = new Button(540, 600, 200, 50, app->font, "AA Tree");
    btnMST = new Button(540, 700, 200, 50, app->font, "Kruskal");

    // Khởi tạo hiệu ứng lá rơi (ví dụ: 80 chiếc lá)
    initLeaves(80);
}

MainMenu::~MainMenu() {
    delete btnSLL;
    delete btnAVL;
    delete btnHeap;
    delete btnAA;
    delete btnMST;
}

void MainMenu::initLeaves(int count) {
    for (int i = 0; i < count; ++i) {
        Leaf leaf;
        
        // Tạo hình dạng lá (hình thoi)
        leaf.shape.setPointCount(4);
        leaf.shape.setPoint(0, sf::Vector2f(0, -10));
        leaf.shape.setPoint(1, sf::Vector2f(5, 0));
        leaf.shape.setPoint(2, sf::Vector2f(0, 10));
        leaf.shape.setPoint(3, sf::Vector2f(-5, 0));

        // Random màu sắc để tạo cảm giác mùa thu (Vàng, Cam, Đỏ nhạt)
        int colorType = rand() % 3;
        if (colorType == 0) {
            leaf.shape.setFillColor(sf::Color(255, 200, 0, 200));      // Vàng
        } else if (colorType == 1) {
            leaf.shape.setFillColor(sf::Color(255, 140, 0, 200));      // Cam
        } else {
            leaf.shape.setFillColor(sf::Color(200, 50, 50, 200));      // Đỏ mùa thu
        }

        // Khởi tạo vị trí ngẫu nhiên (Rải từ trên xuống)
        float startX = static_cast<float>(rand() % 1920);              // Giả sử chiều rộng max là 1920
        float startY = static_cast<float>((rand() % 1080) - 1080);     // Để lá rớt từ từ vào màn hình
        leaf.shape.setPosition(startX, startY);

        // Khởi tạo ngẫu nhiên các tham số vật lý
        leaf.speedY = 40.0f + static_cast<float>(rand() % 80);         // Tốc độ rơi
        leaf.baseSpeedX = -15.0f + static_cast<float>(rand() % 30);    // Hướng bay ngang
        leaf.rotationSpeed = -90.0f + static_cast<float>(rand() % 180);// Tốc độ xoay vòng
        leaf.timeOffset = static_cast<float>(rand() % 100);            // Lệch pha cho hàm sin

        // Tỉ lệ to nhỏ ngẫu nhiên cho tự nhiên
        float scale = 0.8f + (rand() % 15) / 10.0f;
        leaf.shape.setScale(scale, scale);

        leaves.push_back(leaf);
    }
}

void MainMenu::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    if (btnSLL->isClicked(event, mousePos)) {
        app->changeState(new SLLScreen(app)); 
    }
    if (btnAVL->isClicked(event, mousePos)) {
        app->changeState(new AVLScreen(app)); 
    }
    if (btnHeap->isClicked(event, mousePos)){
        app->changeState(new HeapScreen(app));
    }
    if (btnAA->isClicked(event, mousePos)){
        app->changeState(new AAScreen(app));
    }
    if (btnMST->isClicked(event, mousePos)){
        app->changeState(new MSTScreen(app));
    }
}

void MainMenu::update(float deltaTime, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    btnSLL->update(mousePos); 
    btnAVL->update(mousePos);
    btnHeap->update(mousePos);
    btnAA->update(mousePos);
    btnMST->update(mousePos);

    // Cập nhật hoạt ảnh cho từng chiếc lá
    for (auto& leaf : leaves) {
        leaf.timeOffset += deltaTime * 2.0f; // Nhịp độ đung đưa
        
        // Tính toán di chuyển: Vận tốc X được dao động bằng hàm sin tạo cảm giác gió thổi lượn
        float deltaX = (leaf.baseSpeedX + std::sin(leaf.timeOffset) * 40.0f) * deltaTime;
        float deltaY = leaf.speedY * deltaTime;

        leaf.shape.move(deltaX, deltaY);
        leaf.shape.rotate(leaf.rotationSpeed * deltaTime);

        // Nếu lá rơi rớt khỏi cạnh dưới màn hình -> reset lại lên trên cùng
        if (leaf.shape.getPosition().y > window.getSize().y + 20) {
            float startX = static_cast<float>(rand() % window.getSize().x);
            leaf.shape.setPosition(startX, -20.0f);
        }
    }
}

void MainMenu::draw(sf::RenderWindow& window) {
    // 1. Vẽ lá rơi trước (Background)
    for (const auto& leaf : leaves) {
        window.draw(leaf.shape);
    }

    // 2. Vẽ các nút lên trên cùng (Foreground)
    btnSLL->draw(window);
    btnAVL->draw(window);
    btnHeap->draw(window);
    btnAA->draw(window);
    btnMST->draw(window);
}