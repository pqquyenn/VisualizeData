#pragma once
#include "ListNode.h"
#include <SFML/Graphics.hpp>

// Định nghĩa chế độ hoạt ảnh đang chạy
enum class AnimMode { NONE, SEARCH, DELETE };

class SinglyLinkedList {
private:
    ListNode* head;
    sf::Font& font;

    float startX = 100.f;
    float startY = 350.f;
    float nodeSpacing = 110.f;

    // Các biến Playback Timer
    bool isPaused = false;
    float timer = 0.0f;
    float delay = 0.8f; 

    // Quản lý Trạng thái Hoạt ảnh (Kiến trúc mới)
    AnimMode currentMode = AnimMode::NONE;
    int targetValue = 0;
    size_t currentStep = 0;
    size_t totalSteps = 0;
    bool nodeWasFound = false;

    // Các hàm nội bộ
    void updateLayout();
    void drawArrow(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end);
    void resetColors();
    void applyColorsByStep(); // Hàm render màu theo bước cực chuẩn
    void performPhysicalDelete();

public:
    SinglyLinkedList(sf::Font& font);
    ~SinglyLinkedList();

    void clear(); 
    void initList(int n); 

    void insertNode(int val); 
    void insertNodeNoAnimation(int val); 

    void startDelete(int val);
    void startSearch(int val);
    
    // Playback control
    void togglePause() { isPaused = !isPaused; }
    bool getPauseStatus() const { return isPaused; }
    void stepForward();
    void stepBackward();

    void updatePosition(float deltaTime);
    void updateAnimation(float deltaTime); 
    void draw(sf::RenderWindow& window);
};