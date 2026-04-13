#include "DijkstraNode.h"

// Giả sử -1 đại diện cho vô cực (INF)
const int INF_VALUE = -1; 

DijkstraNode::DijkstraNode(int nodeId, sf::Font& font) : id(nodeId), isDragging(false), cost(INF_VALUE), costString("INF") {
    shape.setRadius(22.f);
    shape.setOrigin(22.f, 22.f);
    shape.setOutlineThickness(3.f);
    shape.setOutlineColor(sf::Color::White);
    currentColor = sf::Color(70, 130, 180); // Màu xanh bích mặc định
    shape.setFillColor(currentColor);

    // Cấu hình chữ ID Node (Chính giữa)
    text.setFont(font);
    text.setString(std::to_string(id));
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);

    // --- CẤU HÌNH CHỮ COST (INF) ---
    costText.setFont(font);
    costText.setString(costString);
    costText.setCharacterSize(16);
    costText.setFillColor(sf::Color::Red); // Màu đỏ cho nổi bật
    sf::FloatRect costBounds = costText.getLocalBounds();
    costText.setOrigin(costBounds.left + costBounds.width / 2.0f, costBounds.top + costBounds.height / 2.0f);
}

void DijkstraNode::setCost(int newCost) {
    cost = newCost;
    costString = std::to_string(cost);
    costText.setString(costString);
    // Cập nhật lại origin để canh giữa mỗi khi số thay đổi
    sf::FloatRect bounds = costText.getLocalBounds();
    costText.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
}

void DijkstraNode::resetCost() {
    cost = INF_VALUE;
    costString = "INF";
    costText.setString(costString);
    sf::FloatRect bounds = costText.getLocalBounds();
    costText.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
}

void DijkstraNode::handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view) {
    sf::Vector2f mouseCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (shape.getGlobalBounds().contains(mouseCoords)) {
            isDragging = true;
            dragOffset = shape.getPosition() - mouseCoords;
        }
    } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        isDragging = false;
    } else if (event.type == sf::Event::MouseMoved) {
        if (isDragging) {
            currentPos = mouseCoords + dragOffset;
        }
    }
}

void DijkstraNode::update(float dt) {
    shape.setPosition(currentPos);
    text.setPosition(currentPos);
    
    // Đặt chữ INF nằm cao hơn hình tròn một chút để dễ nhìn
    costText.setPosition(currentPos.x, currentPos.y - 35.f); 
}

void DijkstraNode::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
    window.draw(costText); // Vẽ thêm chi phí
}