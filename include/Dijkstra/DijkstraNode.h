#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class DijkstraNode {
public:
    int id;
    sf::CircleShape shape;
    sf::Text text;       
    
    int cost;            
    std::string costString; 
    sf::Text costText;   

    sf::Vector2f currentPos;
    sf::Color currentColor; // Sẽ được cập nhật liên tục từ Graph

    bool isDragging;
    sf::Vector2f dragOffset;

    DijkstraNode(int nodeId, sf::Font& font);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view);
    
    void setCost(int newCost);
    void resetCost();
};