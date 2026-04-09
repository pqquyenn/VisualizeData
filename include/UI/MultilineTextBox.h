#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class MultilineTextBox {
private:
    sf::RectangleShape box;
    sf::Text text;
    std::string inputString;
    bool isSelected;
    sf::Clock cursorClock; // THÊM DÒNG NÀY: Dùng để làm con trỏ nhấp nháy

public:
    MultilineTextBox(float x, float y, float width, float height, sf::Font& font);

    void handleEvent(sf::Event& event, sf::Vector2f mousePos);
    void draw(sf::RenderWindow& window);

    std::string getText() const;
    void clear();
};