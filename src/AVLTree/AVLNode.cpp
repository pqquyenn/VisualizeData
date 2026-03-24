#include "AVLNode.h"
#include <cmath> // Để dùng tính toán khoảng cách

AVLNode::AVLNode(int value, sf::Font& font) {
    val = value;
    height = 1;
    left = nullptr;
    right = nullptr;

    // Thiết lập hình tròn
    float radius = 25.0f;
    circle.setRadius(radius);
    currentColor = sf::Color(70, 130, 180); // Màu xanh dương mặc định
    circle.setFillColor(currentColor);
    circle.setOutlineThickness(3.0f);
    circle.setOutlineColor(sf::Color::White);
    circle.setOrigin(radius, radius); // Đặt tâm ở giữa hình tròn

    // Thiết lập Text
    text.setFont(font);
    text.setString(std::to_string(val));
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    
    // Canh giữa text
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
}

AVLNode::~AVLNode() {}

void AVLNode::updateAnimation(float deltaTime) {
    // Di chuyển mượt (Lerp) từ currentPos đến targetPos
    float speed = 5.0f; // Tốc độ bay
    currentPos.x += (targetPos.x - currentPos.x) * speed * deltaTime;
    currentPos.y += (targetPos.y - currentPos.y) * speed * deltaTime;

    circle.setPosition(currentPos);
    text.setPosition(currentPos);
    circle.setFillColor(currentColor); // Cập nhật màu hiện tại
}

void AVLNode::draw(sf::RenderWindow& window) {
    // Vẽ đường thẳng nối với con trái
    if (left != nullptr) {
        sf::Vertex line[] = {
            sf::Vertex(currentPos, sf::Color::White),
            sf::Vertex(left->currentPos, sf::Color::White)
        };
        window.draw(line, 2, sf::Lines);
    }
    // Vẽ đường thẳng nối với con phải
    if (right != nullptr) {
        sf::Vertex line[] = {
            sf::Vertex(currentPos, sf::Color::White),
            sf::Vertex(right->currentPos, sf::Color::White)
        };
        window.draw(line, 2, sf::Lines);
    }

    // Vẽ Node đè lên các đường thẳng
    window.draw(circle);
    window.draw(text);
}