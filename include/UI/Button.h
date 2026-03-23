#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Color normalColor;
    sf::Color hoverColor;
    bool isHovering;

public:
    Button(float x, float y, float width, float height, sf::Font& font, std::string btnText);
    
    // Cập nhật trạng thái hover (chuột lướt qua)
    void update(sf::Vector2f mousePos);
    void draw(sf::RenderWindow& window);
    
    // Kiểm tra xem nút có được click hay không
    bool isClicked(sf::Event& event, sf::Vector2f mousePos);
};