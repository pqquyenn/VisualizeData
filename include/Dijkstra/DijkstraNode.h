#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class DijkstraNode {
public:
    int id;
    sf::CircleShape shape;
    sf::Text text;       // Text hiển thị ID của Node
    
    // --- THÊM CHO DIJKSTRA ---
    int cost;            // Chi phí đường đi từ Source tới Node này
    std::string costString; 
    sf::Text costText;   // Text hiển thị chi phí (INF hoặc số)

    sf::Vector2f currentPos;
    sf::Color currentColor;

    bool isDragging;
    sf::Vector2f dragOffset;

    DijkstraNode(int nodeId, sf::Font& font);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view);
    
    // Hàm hỗ trợ cập nhật chi phí hiển thị
    void setCost(int newCost);
    void resetCost();
};