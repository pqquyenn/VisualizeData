#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class HeapNode {
public:
    sf::CircleShape shape;
    sf::Text text;
    int value;

    sf::Vector2f currentPos;
    sf::Vector2f targetPos;
    sf::Vector2f dragOffset; // Độ lệch khi dùng chuột kéo node

    sf::Color currentColor;
    sf::Color targetColor;

    bool isActive;
    bool isDragging; // Trạng thái có đang bị chuột giữ hay không

    HeapNode(int val, sf::Font& font);

    void setValue(int val);
    void update(float dt);
    void draw(sf::RenderWindow& window);
};