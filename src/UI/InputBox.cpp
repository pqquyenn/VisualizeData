#include "InputBox.h"

InputBox::InputBox(float x, float y, float width, float height, sf::Font& font, int limit) {
    this->limit = limit;
    isSelected = false;

    // Cài đặt khung viền chữ nhật
    box.setPosition(sf::Vector2f(x, y));
    box.setSize(sf::Vector2f(width, height));
    box.setFillColor(sf::Color::White);
    box.setOutlineThickness(2.f);
    box.setOutlineColor(sf::Color(150, 150, 150)); // Viền xám mặc định

    // Cài đặt chữ hiển thị bên trong
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Black);
    text.setPosition(x + 10.f, y + (height / 2.0f) - 12.f); // Căn lề chữ
    inputString = "";
}

void InputBox::handleEvent(sf::Event& event, sf::Vector2f mousePos) {
    // 1. Kiểm tra xem người dùng có click vào ô này không
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (box.getGlobalBounds().contains(mousePos)) {
            isSelected = true;
            box.setOutlineColor(sf::Color::Blue); // Đổi viền xanh khi được chọn
        } else {
            isSelected = false;
            box.setOutlineColor(sf::Color(150, 150, 150)); // Trả lại viền xám
        }
    }

    // 2. Nếu đang được chọn thì mới bắt sự kiện gõ phím
    if (isSelected && event.type == sf::Event::TextEntered) {
// Mã ASCII 8 là Backspace, 127 là Delete
        if (event.text.unicode == 8 || event.text.unicode == 127) {
            if (inputString.length() > 0) {
                inputString.pop_back(); 
            }
        }
        // Chỉ cho phép nhập số (từ '0' đến '9')
        else if (event.text.unicode >= '0' && event.text.unicode <= '9') {
            if (inputString.length() < limit) {
                inputString += static_cast<char>(event.text.unicode);
            }
        }
        // Cập nhật lại Text để vẽ ra màn hình
        text.setString(inputString);
    }
}

void InputBox::draw(sf::RenderWindow& window) {
    window.draw(box);
    window.draw(text);
}

std::string InputBox::getText() const {
    return inputString;
}

void InputBox::clear() {
    inputString = "";
    text.setString("");
}

void InputBox::setSelected(bool selected) {
    isSelected = selected;
    box.setOutlineColor(selected ? sf::Color::Blue : sf::Color(150, 150, 150));
}