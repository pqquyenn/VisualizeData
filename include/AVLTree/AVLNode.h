#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <map>

class AVLNode {
public:
    int value;
    bool isActive; // Ẩn/hiện node tuỳ thuộc vào trạng thái step
    int leftVal;   // Giữ value của con trái để vẽ mũi tên (-1 nếu không có)
    int rightVal;  // Giữ value của con phải để vẽ mũi tên (-1 nếu không có)

    sf::CircleShape shape;
    sf::Text text;

    sf::Vector2f currentPos;
    sf::Vector2f targetPos;
    sf::Color currentColor;
    sf::Color targetColor;

    AVLNode(int val, sf::Font& font);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void drawArrows(sf::RenderWindow& window, std::map<int, AVLNode*>& allNodes);
};