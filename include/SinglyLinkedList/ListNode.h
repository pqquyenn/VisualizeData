// #pragma once
// #include <string>
// #include <SFML/Graphics.hpp>
// #include <SFML/System.hpp>
// #include <SFML/Audio.hpp>
// #include <SFML/Network.hpp>
// #include <SFML/Config.hpp>

// class ListNode{
//     public:
//         int val;
//         ListNode* next;
//         // appearance of node
//         sf::CircleShape shape;
//         sf::Text text;
//         // Phai mot ham mau de kiem tra mau khoi ton cong phai check
//         sf::Color currentcolor;
//         sf::Vector2f targetPosition;
//     public:
//         ListNode();
//         ListNode(int x, sf::Font &font, sf::Vector2f startPos);
//         // cap nhat trang thai chuyen mau
//         void update(float deltatime);
//         // ve hinh ra window
//         void renderNode(sf::RenderWindow &window);
//         // xanh khi dang duyet, do khi da tim thay
//         void setColor(sf::Color color);
// };

#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class ListNode {
public:
    int val;
    ListNode* next;
    
    // Appearance of node
    sf::CircleShape shape;
    sf::Text text;
    sf::Color currentcolor;
    sf::Vector2f targetPosition;

public:
    // Khai báo các hàm khởi tạo
    ListNode(); 
    ListNode(int x, sf::Font &font, sf::Vector2f startPos);
    
    // Các hàm chức năng
    void update(float deltatime);
    void renderNode(sf::RenderWindow &window);
    void setColor(sf::Color color);
};