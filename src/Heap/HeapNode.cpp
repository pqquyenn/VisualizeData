#include "HeapNode.h"
#include <cmath>

HeapNode::HeapNode(int val, sf::Font& font) 
    : isActive(false), isDragging(false) 
{
    float radius = 22.0f; 
    shape.setRadius(radius);
    shape.setOrigin(radius, radius); 
    shape.setOutlineThickness(3.0f);
    shape.setOutlineColor(sf::Color::White);
    
    currentColor = targetColor = sf::Color(70, 130, 180); 
    shape.setFillColor(currentColor);

    text.setFont(font);
    text.setCharacterSize(18); 
    text.setFillColor(sf::Color::White);
    
    setValue(val);
}

void HeapNode::setValue(int val) {
    value = val;
    text.setString(std::to_string(val));
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(std::round(bounds.left + bounds.width / 2.0f), 
                   std::round(bounds.top + bounds.height / 2.0f));
}

void HeapNode::update(float dt) {
    if (!isActive) return;

    if (!isDragging) {
        currentPos += (targetPos - currentPos) * 6.0f * dt;
    }

    shape.setPosition(currentPos);
    text.setPosition(currentPos);

    int r = currentColor.r + (targetColor.r - currentColor.r) * 5.0f * dt;
    int g = currentColor.g + (targetColor.g - currentColor.g) * 5.0f * dt;
    int b = currentColor.b + (targetColor.b - currentColor.b) * 5.0f * dt;
    currentColor = sf::Color(r, g, b);
    shape.setFillColor(currentColor);
}

void HeapNode::draw(sf::RenderWindow& window) {
    if (isActive) {
        window.draw(shape);
        window.draw(text);
    }
}