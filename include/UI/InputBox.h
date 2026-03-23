#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class InputBox {
private:
    sf::RectangleShape box;
    sf::Text text;
    std::string inputString;
    bool isSelected;    // Trạng thái ô có đang được click chọn không
    int limit;          // Giới hạn số lượng ký tự được nhập

public:
    InputBox(float x, float y, float width, float height, sf::Font& font, int limit = 5);

    // Xử lý click chuột và gõ phím
    void handleEvent(sf::Event& event, sf::Vector2f mousePos);
    
    void draw(sf::RenderWindow& window);

    std::string getText() const; // Lấy chuỗi người dùng đã nhập
    void clear();                // Xóa trắng ô nhập
    
    void setSelected(bool selected); // Bật/tắt trạng thái chọn
};