#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <map>

class AANode {
public:
    int value;
    bool isActive;
    int leftVal;
    int rightVal;

    sf::CircleShape shape;
    sf::Text text;

    sf::Vector2f currentPos;
    sf::Vector2f targetPos;
    sf::Color currentColor;
    sf::Color targetColor;

    bool isDragging; 
    sf::Vector2f dragOffset; 

    AANode(int val, sf::Font& font);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void drawArrows(sf::RenderWindow& window, std::map<int, AANode*>& allNodes);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view);
};