#include "ListNode.h"
#include <cmath>

ListNode::ListNode(int id, int val, sf::Font& font) : id(id), value(val), isActive(false), nextId(-1) {
    shape.setRadius(22.f);
    shape.setOrigin(22.f, 22.f);
    shape.setOutlineThickness(3.f);
    shape.setOutlineColor(sf::Color::White);
    currentColor = sf::Color(70, 130, 180);
    targetColor = currentColor;
    shape.setFillColor(currentColor);

    text.setFont(font);
    text.setString(std::to_string(val));
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
}

void ListNode::update(float dt) {
    if (!isActive) return;
    currentPos += (targetPos - currentPos) * 6.0f * dt;
    shape.setPosition(currentPos);
    text.setPosition(currentPos);

    int r = currentColor.r + (targetColor.r - currentColor.r) * 5.0f * dt;
    int g = currentColor.g + (targetColor.g - currentColor.g) * 5.0f * dt;
    int b = currentColor.b + (targetColor.b - currentColor.b) * 5.0f * dt;
    currentColor = sf::Color(r, g, b);
    shape.setFillColor(currentColor);
}

void ListNode::draw(sf::RenderWindow& window) {
    if (isActive) {
        window.draw(shape);
        window.draw(text);
    }
}

void ListNode::drawArrow(sf::RenderWindow& window, std::map<int, ListNode*>& allNodes) {
    if (!isActive || nextId == -1 || !allNodes.count(nextId) || !allNodes[nextId]->isActive) return;

    sf::Vector2f start = currentPos;
    sf::Vector2f end = allNodes[nextId]->currentPos;

    float dist = std::sqrt(std::pow(end.x - start.x, 2) + std::pow(end.y - start.y, 2));
    if (dist > 50.f) {
        sf::Vector2f dir = (end - start) / dist;
        sf::Vector2f adjustedStart = start + dir * 22.f; 
        sf::Vector2f adjustedEnd = end - dir * 25.f; 

        sf::Vertex line[] = { sf::Vertex(adjustedStart, sf::Color::White), sf::Vertex(adjustedEnd, sf::Color::White) };
        window.draw(line, 2, sf::Lines);
        
        float angle = std::atan2(end.y - start.y, end.x - start.x);
        sf::CircleShape arrowHead(5, 3);
        arrowHead.setFillColor(sf::Color::White);
        arrowHead.setOrigin(5, 5);
        arrowHead.setPosition(adjustedEnd);
        arrowHead.setRotation(angle * 180 / 3.14159f + 90);
        window.draw(arrowHead);
    }
}