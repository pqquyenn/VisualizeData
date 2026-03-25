#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// --- FILE: include/Heap/HeapNode.h ---
// Định nghĩa Node trực quan của Max Heap, tích hợp tính năng kéo thả.

class HeapNode {
public:
    int value;
    bool isActive; // Ẩn/hiện node tuỳ thuộc vào trạng thái playback

    sf::CircleShape shape;
    sf::Text text;

    // Phục vụ Animation (Lerp - Nội suy tuyến tính)
    sf::Vector2f currentPos;  // Vị trí hiện tại
    sf::Vector2f targetPos;   // Vị trí đích (logic)
    sf::Color currentColor;   // Màu hiện tại
    sf::Color targetColor;    // Màu đích (ví dụ: vàng khi so sánh)

    // --- TÍNH NĂNG KÉO THẢ (Sử dụng logic từ AVLNode của bạn) ---
    bool isDragging; 
    sf::Vector2f dragOffset; // Khoảng cách từ chuột đến tâm node khi bắt đầu kéo

    HeapNode(int val, sf::Font& font);
    ~HeapNode() = default;

    void update(float dt); // Cập nhật vị trí và màu sắc mịn
    void draw(sf::RenderWindow& window); // Vẽ node

    // Xử lý sự kiện kéo thả chuột, cần dùng View để map tọa độ chính xác
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view);
};