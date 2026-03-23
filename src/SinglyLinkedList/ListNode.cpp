// #include "ListNode.h"
// #include <string>
// #include <cmath>

// ListNode::ListNode(int x, sf::Font &font, sf::Vector2f startPos){
//     this->val = x;
//     this->next = nullptr;

//     // thiet lap shape
//     float radius = 30.f;
//     this->shape.setRadius(radius);
//     this->shape.setOrigin(radius, radius);
//     this->shape.setPosition(startPos);
//     this->shape.setFillColor(sf::Color(7, 67, 102));
//     this->shape.setOutlineThickness(1.f);
//     this->shape.setOutlineColor(sf::Color::Black);
//     // thiet lap text;
//     this->text.setFont(font);
//     this->text.setString(std::to_string(x));
//     this->text.setCharacterSize(30.f);
//     this->text.setFillColor(sf::Color(166, 150, 10));
//     // tinh toan de can giua text
//     sf::FloatRect textrect = (this->text).getLocalBounds();
//     // change the origin and Position of the 
//     this->text.setOrigin(textrect.left + textrect.width/2.f, textrect.top + textrect.height/2.f);
//     this->text.setPosition(startPos);
//     this->targetPosition = startPos;
// }

// void ListNode::update(float deltatime){
//     sf::Vector2f currentPos = shape.getPosition();
//     float dx = targetPosition.x - currentPos.x;
//     float dy = targetPosition.y - currentPos.y;
//     // float speed = 10.f;
//     // const float pi = 3.14;
//     // if (abs(dx) > 0.5f || abs(dy) > 0.5f){
//     //     float newx = currentPos.x + speed * (1.0/deltatime);
//     //     float newy = currentPos.y + speed * (1.0/deltatime);
//     //     // 
//     //     shape.setPosition(newx, newy);
//     //     text.setPosition(newx, newy); 
//     // }
//     // else{
//         // Khi đã đến rất gần đích (sai số < 0.5), ta ép nó vào đúng tọa độ đích luôn.
//         // Điều này giúp tránh việc Node bị "rung lắc" do sai số số học (floating-point).
//         shape.setPosition(targetPosition);
//         text.setPosition(targetPosition);
//     // }
// }

// void ListNode::renderNode(sf::RenderWindow &window){
//     window.draw(this->shape);
//     window.draw(this->text);
// }

// void ListNode::setColor(sf::Color color){
//     this->currentcolor = color;
//     this->shape.setFillColor(color);
// }

#include "ListNode.h"
#include <cmath>

// ĐÂY CHÍNH LÀ PHẦN BỊ THIẾU KHIẾN PROJECT KHÔNG CHẠY
ListNode::ListNode() {
    this->val = 0;
    this->next = nullptr;
}

// Hàm khởi tạo có tham số của bạn (Giữ nguyên logic cực chuẩn của bạn)
ListNode::ListNode(int x, sf::Font &font, sf::Vector2f startPos) {
    this->val = x;
    this->next = nullptr;

    // Thiet lap shape
    float radius = 30.f;
    this->shape.setRadius(radius);
    this->shape.setOrigin(radius, radius);
    this->shape.setPosition(startPos);
    this->shape.setFillColor(sf::Color(7, 67, 102));
    this->shape.setOutlineThickness(1.f);
    this->shape.setOutlineColor(sf::Color::Black);
    
    // Thiet lap text
    this->text.setFont(font);
    this->text.setString(std::to_string(x));
    this->text.setCharacterSize(30.f);
    this->text.setFillColor(sf::Color(166, 150, 10));
    
    // Tinh toan de can giua text
    sf::FloatRect textrect = (this->text).getLocalBounds();
    this->text.setOrigin(textrect.left + textrect.width / 2.f, textrect.top + textrect.height / 2.f);
    this->text.setPosition(startPos);
    
    this->targetPosition = startPos;
}

void ListNode::update(float deltatime) {
    // Tạm thời ép tọa độ về đích luôn để tránh rung lắc như bạn comment
    shape.setPosition(targetPosition);
    text.setPosition(targetPosition);
}

void ListNode::renderNode(sf::RenderWindow &window) {
    window.draw(this->shape);
    window.draw(this->text);
}

void ListNode::setColor(sf::Color color) {
    this->currentcolor = color;
    this->shape.setFillColor(color);
}