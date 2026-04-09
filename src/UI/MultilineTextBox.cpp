#include "MultilineTextBox.h"

MultilineTextBox::MultilineTextBox(float x, float y, float width, float height, sf::Font& font) {
    isSelected = false;

    box.setPosition(sf::Vector2f(x, y));
    box.setSize(sf::Vector2f(width, height));
    box.setFillColor(sf::Color::White);
    box.setOutlineThickness(2.f);
    box.setOutlineColor(sf::Color(150, 150, 150));

    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::Black);
    text.setPosition(x + 10.f, y + 10.f); 
    inputString = "";
}

void MultilineTextBox::handleEvent(sf::Event& event, sf::Vector2f mousePos) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (box.getGlobalBounds().contains(mousePos)) {
            isSelected = true;
            box.setOutlineColor(sf::Color::Blue);
        } else {
            isSelected = false;
            box.setOutlineColor(sf::Color(150, 150, 150));
        }
    }

    if (isSelected && event.type == sf::Event::TextEntered) {
        sf::Uint32 unicode = event.text.unicode;

        if (unicode == 8 || unicode == 127) { // Backspace
            if (!inputString.empty()) {
                inputString.pop_back();
            }
        } 
        else if (unicode == 13) { // Phím Enter (xuống dòng)
            inputString += '\n';
        }
        else if (unicode == 32) { // Phím Space
            inputString += ' ';
        }
        else if (unicode >= '0' && unicode <= '9') { // Chỉ cho phép nhập số
            inputString += static_cast<char>(unicode);
        }
    }
}

void MultilineTextBox::draw(sf::RenderWindow& window) {
    window.draw(box);
    
    // XỬ LÝ CON TRỎ NHẤP NHÁY
    std::string displayText = inputString;
    if (isSelected) {
        // Cứ mỗi nửa giây (500ms) thì hiện dấu '|'
        if (cursorClock.getElapsedTime().asMilliseconds() % 1000 < 500) {
            displayText += "|"; 
        }
    }
    
    text.setString(displayText);
    window.draw(text);
}

std::string MultilineTextBox::getText() const { return inputString; }
void MultilineTextBox::clear() { inputString = ""; text.setString(""); }