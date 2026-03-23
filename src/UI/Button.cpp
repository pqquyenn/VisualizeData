#include "Button.h"

Button::Button(float x, float y, float width, float height, sf::Font& font, std::string btnText) {
    shape.setPosition(sf::Vector2f(x, y));
    shape.setSize(sf::Vector2f(width, height));
    
    normalColor = sf::Color(70, 130, 180); // Xanh dương
    hoverColor = sf::Color(100, 149, 237); // Xanh sáng hơn khi hover
    shape.setFillColor(normalColor);

    text.setFont(font);
    text.setString(btnText);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);

    // Căn giữa chữ vào nút
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(x + width / 2.0f, y + height / 2.0f);

    isHovering = false;
}

void Button::update(sf::Vector2f mousePos) {
    // Kiem tra xem chuot co nam trong nut khong
    if (shape.getGlobalBounds().contains(mousePos)) {
        shape.setFillColor(hoverColor);
        isHovering = true;
    } else {
        shape.setFillColor(normalColor);
        isHovering = false;
    }
}

bool Button::isClicked(sf::Event& event, sf::Vector2f mousePos) {
    // Sửa thành kiểm tra getGlobalBounds().contains(mousePos) trực tiếp
    if (shape.getGlobalBounds().contains(mousePos) && 
        event.type == sf::Event::MouseButtonPressed && 
        event.mouseButton.button == sf::Mouse::Left) {
        return true;
    }
    return false;
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}