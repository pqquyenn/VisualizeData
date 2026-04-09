#include "MSTNode.h"

MSTNode::MSTNode(int nodeId, sf::Font& font) : id(nodeId), isDragging(false) {
    shape.setRadius(22.f);
    shape.setOrigin(22.f, 22.f);
    shape.setOutlineThickness(3.f);
    shape.setOutlineColor(sf::Color::White);
    currentColor = sf::Color(70, 130, 180);
    shape.setFillColor(currentColor);

    text.setFont(font);
    text.setString(std::to_string(id));
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
}

void MSTNode::handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view) {
    sf::Vector2f mouseCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (shape.getGlobalBounds().contains(mouseCoords)) {
            isDragging = true;
            dragOffset = shape.getPosition() - mouseCoords;
        }
    } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        isDragging = false;
    } else if (event.type == sf::Event::MouseMoved) {
        if (isDragging) {
            currentPos = mouseCoords + dragOffset;
        }
    }
}

void MSTNode::update(float dt) {
    shape.setPosition(currentPos);
    text.setPosition(currentPos);
}

void MSTNode::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}