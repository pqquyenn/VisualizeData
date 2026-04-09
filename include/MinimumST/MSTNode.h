#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class MSTNode {
public:
    int id;
    sf::CircleShape shape;
    sf::Text text;

    sf::Vector2f currentPos;
    sf::Color currentColor;

    bool isDragging;
    sf::Vector2f dragOffset;

    MSTNode(int nodeId, sf::Font& font);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view);
};