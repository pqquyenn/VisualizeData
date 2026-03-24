#include "ListNode.h"

ListNode::ListNode(int val, sf::Font& font) : value(val), next(nullptr) {
    float radius = 30.f;
    shape.setRadius(radius);
    shape.setOrigin(radius, radius); 
    shape.setOutlineThickness(3.f);
    
    currentColor = sf::Color(70, 130, 180); // Xanh dương
    shape.setFillColor(currentColor);
    shape.setOutlineColor(sf::Color::White);

    text.setFont(font);
    text.setString(std::to_string(value));
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);

    // Xuất phát điểm từ trên cao rơi xuống
    currentPos = sf::Vector2f(100, -100); 
    targetPos = sf::Vector2f(100, -100);
}

void ListNode::update(float dt) {
    float moveSpeed = 6.0f; // Tốc độ di chuyển animation
    currentPos.x += (targetPos.x - currentPos.x) * moveSpeed * dt;
    currentPos.y += (targetPos.y - currentPos.y) * moveSpeed * dt;

    shape.setPosition(currentPos);
    text.setPosition(currentPos);
    shape.setFillColor(currentColor);
}

void ListNode::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

void ListNode::setTargetPosition(sf::Vector2f pos) {
    targetPos = pos;
}

void ListNode::setColor(sf::Color color) {
    currentColor = color;
}