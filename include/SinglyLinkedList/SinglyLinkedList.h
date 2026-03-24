#pragma once
#include "ListNode.h"
#include <SFML/Graphics.hpp>
#include <vector>

class SinglyLinkedList {
private:
    ListNode* head;
    sf::Font& font;

    float startX = 100.f;
    float startY = 350.f;
    float nodeSpacing = 110.f;

    // Playback control states
    bool isPaused = false;
    size_t animationStepCount = 0; // Current step count
    float timer = 0.0f;
    float delay = 0.8f; 

// Search Animation specific
    bool isSearching = false;
    int searchValue = 0;
    ListNode* currentSearchNode = nullptr;
    ListNode* previousSearchNode = nullptr; // <--- THÊM DÒNG NÀY VÀO

    // Delete Animation specific
    bool isDeleting = false;
    bool isConfirmingDeletion = false; // Phase to show found node as red
    int deleteValue = 0;
    ListNode* currentDeleteNode = nullptr;
    ListNode* previousDeleteNode = nullptr;

    void updateLayout();
    void drawArrow(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end);
    void resetColors();
    void resetColorsToStep(); // Re-apply colors based on step count for backtracking
    ListNode* getNodeAt(ListNode* h, size_t index);

    // Step logic functions
    void handleSearchStep();
    void handleSearchBack();
    void handleDeleteStep();
    void handleDeleteBack();

public:
    SinglyLinkedList(sf::Font& font);
    ~SinglyLinkedList();

    void clear(); // Helper to remove all nodes
    void initList(int n); // Bulk random initialization

    void insertNode(int val); // Falling animation
    void insertNodeNoAnimation(int val); // Direct final position

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