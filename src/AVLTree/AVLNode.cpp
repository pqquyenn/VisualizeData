#include "AVLNode.h"
#include <cmath>

AVLNode::AVLNode(int val, sf::Font& font) : value(val), isActive(false), leftVal(-1), rightVal(-1), isDragging(false) { // Initialize isDragging
    shape.setRadius(22.f);
    shape.setOrigin(22.f, 22.f);
    shape.setOutlineThickness(3.f);
    shape.setOutlineColor(sf::Color::White);
    currentColor = sf::Color(70, 130, 180);
    targetColor = currentColor;
    shape.setFillColor(currentColor);

    text.setFont(font);
    text.setString(std::to_string(val));
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
}

// --- THÊM: Xử lý sự kiện kéo thả ---
void AVLNode::handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view) {
    if (!isActive) return;

    // Chuyển đổi vị trí chuột sang toạ độ thế giới (coords) bằng cách sử dụng view
    sf::Vector2f mouseCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        // Kiểm tra xem chuột có nằm trong naode không
        if (shape.getGlobalBounds().contains(mouseCoords)) {
            isDragging = true;
            dragOffset = shape.getPosition() - mouseCoords;
        }
    } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        isDragging = false;
    } else if (event.type == sf::Event::MouseMoved) {
        if (isDragging) {
            // Cập nhật vị trí node ngay lập tức theo chuột
            currentPos = mouseCoords + dragOffset;
            targetPos = currentPos; // Cập nhật targetPos để node không quay lại vị trí cũ
        }
    }
}

void AVLNode::update(float dt) {
    if (!isActive) return;

    // --- SỬA: Bỏ qua nội suy khi đang kéo thả để di chuyển tức thời ---
    if (!isDragging) {
        currentPos += (targetPos - currentPos) * 6.0f * dt;
    }

    shape.setPosition(currentPos);
    text.setPosition(currentPos);

    // Nội suy màu sắc mượt mà
    int r = currentColor.r + (targetColor.r - currentColor.r) * 5.0f * dt;
    int g = currentColor.g + (targetColor.g - currentColor.g) * 5.0f * dt;
    int b = currentColor.b + (targetColor.b - currentColor.b) * 5.0f * dt;
    currentColor = sf::Color(r, g, b);
    shape.setFillColor(currentColor);
}

void AVLNode::draw(sf::RenderWindow& window) {
    if (isActive) {
        window.draw(shape);
        window.draw(text);
    }
}

void AVLNode::drawArrows(sf::RenderWindow& window, std::map<int, AVLNode*>& allNodes) {
    if (!isActive) return;
    auto drawLine = [&](sf::Vector2f start, sf::Vector2f end) {
        sf::Vertex line[] = { sf::Vertex(start, sf::Color::White), sf::Vertex(end, sf::Color::White) };
        window.draw(line, 2, sf::Lines);
    };
    if (leftVal != -1 && allNodes.count(leftVal) && allNodes[leftVal]->isActive) {
        drawLine(currentPos, allNodes[leftVal]->currentPos);
    }
    if (rightVal != -1 && allNodes.count(rightVal) && allNodes[rightVal]->isActive) {
        drawLine(currentPos, allNodes[rightVal]->currentPos);
    }
}