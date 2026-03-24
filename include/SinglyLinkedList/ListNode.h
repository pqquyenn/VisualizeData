#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class ListNode {
public:
    int value;
    ListNode* next;

    sf::CircleShape shape;
    sf::Text text;         

    // Tọa độ để làm animation mượt
    sf::Vector2f currentPos;
    sf::Vector2f targetPos;
    
    // Màu sắc để làm animation search
    sf::Color currentColor;

    ListNode(int val, sf::Font& font);
    
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void setTargetPosition(sf::Vector2f pos);
    void setColor(sf::Color color);
};