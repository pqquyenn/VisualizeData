#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class AVLNode {
public:
    int val;
    int height;
    AVLNode* left;
    AVLNode* right;

    // Đồ họa
    sf::CircleShape circle;
    sf::Text text;
    
    // Phục vụ Animation di chuyển mượt mà
    sf::Vector2f currentPos;
    sf::Vector2f targetPos;
    
    // Màu sắc hiện tại (để dễ lưu lịch sử)
    sf::Color currentColor;

    AVLNode(int value, sf::Font& font);
    ~AVLNode();

    // Hàm cập nhật animation (di chuyển từ currentPos tới targetPos)
    void updateAnimation(float deltaTime);
    void draw(sf::RenderWindow& window);
};