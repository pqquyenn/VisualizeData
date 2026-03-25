#include "HeapNode.h"
#include <cmath>

// --- FILE: src/Heap/HeapNode.cpp ---

HeapNode::HeapNode(int val, sf::Font& font) 
    : value(val), isActive(false), isDragging(false) 
{
    float radius = 22.0f; // Kích thước đồng bộ với AVL
    shape.setRadius(radius);
    shape.setOrigin(radius, radius); // Gốc tọa độ ở tâm đường tròn
    shape.setOutlineThickness(3.0f);
    shape.setOutlineColor(sf::Color::White);
    
    // Màu xanh chuẩn mặc định
    currentColor = targetColor = sf::Color(70, 130, 180); 
    shape.setFillColor(currentColor);

    text.setFont(font);
    text.setString(std::to_string(val));
    text.setCharacterSize(18); // Kích thước chữ đồng bộ
    text.setFillColor(sf::Color::White);
    
    // --- SỬA LỖI: Căn giữa văn bản trong Node ---
    // Phải lấy getLocalBounds() và setOrigin về tâm văn bản
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
}

void HeapNode::handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view) {
    if (!isActive) return;

    // Chuyển đổi vị trí pixel của chuột sang tọa độ thế giới (trong Tree View)
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseCoords = window.mapPixelToCoords(pixelPos, view);

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        // Kiểm tra xem chuột có nằm trong node này không
        if (shape.getGlobalBounds().contains(mouseCoords)) {
            isDragging = true;
            // Tính toán khoảng cách để kéo mượt
            dragOffset = shape.getPosition() - mouseCoords;
        }
    } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        isDragging = false;
    } else if (event.type == sf::Event::MouseMoved && isDragging) {
        // --- TÍNH NĂNG KÉO THẢ: Di chuyển node tức thời theo chuột ---
        currentPos = mouseCoords + dragOffset;
        targetPos = currentPos; // Cập nhật đích để node không bị kéo ngược lại
    }
}

void HeapNode::update(float dt) {
    if (!isActive) return;

    // --- SỬA: Bỏ qua nội suy khi đang kéo thả (Logic từ AVLNode của bạn) ---
    // Khi đang kéo, node di chuyển tức thời theo chuột trong handleEvent
    if (!isDragging) {
        // Di chuyển mượt mà đến vị trí đích (tốc độ 6.0f mượt mà giống AVL)
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

void HeapNode::draw(sf::RenderWindow& window) {
    if (isActive) {
        window.draw(shape);
        window.draw(text);
    }
}