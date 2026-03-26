#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <map>

class ListNode {
public:
    int id;
    int value;
    bool isActive; 
    int nextId;

    sf::CircleShape shape;
    sf::Text text;

    sf::Vector2f currentPos;
    sf::Vector2f targetPos;
    sf::Color currentColor;
    sf::Color targetColor;

    ListNode(int id, int val, sf::Font& font);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void drawArrow(sf::RenderWindow& window, std::map<int, ListNode*>& allNodes);
};