#include "DijkstraNode.h"

const int INF_VALUE = -1; 

DijkstraNode::DijkstraNode(int nodeId, sf::Font& font) : id(nodeId), isDragging(false), cost(INF_VALUE), costString("INF") {
    shape.setRadius(22.f);
    shape.setOrigin(22.f, 22.f);
    shape.setOutlineThickness(3.f);
    shape.setOutlineColor(sf::Color::White);
    currentColor = sf::Color(70, 130, 180); // Blue mặc định
    shape.setFillColor(currentColor);

    text.setFont(font);
    text.setString(std::to_string(id));
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);

    // --- CHỈNH SỬA PHẦN NÀY ĐỂ LÀM NỔI BẬT COST TEXT ---
    costText.setFont(font);
    costText.setString(costString);
    
    // 1. Tăng kích thước chữ (từ 16 lên 24 hoặc 28 tùy ý bạn)
    costText.setCharacterSize(26); 
    
    // 2. In đậm chữ
    costText.setStyle(sf::Text::Bold); 
    
    // 3. Chọn một màu thật nổi bật (Ví dụ: Màu Cyan/Xanh lơ mạ)
    costText.setFillColor(sf::Color::Cyan); 
    
    // 4. Thêm viền đen để chữ luôn rõ nét dù đè lên cạnh (Edge) màu gì
    costText.setOutlineColor(sf::Color::Black);
    costText.setOutlineThickness(2.5f); 

    // Căn giữa origin
    sf::FloatRect costBounds = costText.getLocalBounds();
    costText.setOrigin(costBounds.left + costBounds.width / 2.0f, costBounds.top + costBounds.height / 2.0f);

    //
    
}

void DijkstraNode::setCost(int newCost) {
    cost = newCost;
    if (cost == INF_VALUE) costString = "INF";
    else costString = std::to_string(cost);
    
    costText.setString(costString);
    sf::FloatRect bounds = costText.getLocalBounds();
    costText.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
}

void DijkstraNode::resetCost() {
    setCost(INF_VALUE);
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
    
    // Cập nhật màu sắc từ biến currentColor
    shape.setFillColor(currentColor);
    
    // Tăng khoảng cách offset Y từ -35.f lên khoảng -45.f hoặc -50.f
    costText.setPosition(currentPos.x, currentPos.y - 48.f); 
}

void DijkstraNode::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
    window.draw(costText); 
}