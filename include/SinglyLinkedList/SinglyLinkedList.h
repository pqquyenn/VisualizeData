#pragma once
#include "ListNode.h"
#include <SFML/Graphics.hpp>

class SinglyLinkedList {
private:
    ListNode* head;
    sf::Font& font;

    float startX = 100.f;
    float startY = 350.f;
    float nodeSpacing = 110.f;

    bool isSearching;
    int searchValue;
    ListNode* currentSearchNode;
    float searchTimer;
    float searchDelay; 

    void updateLayout();
    void drawArrow(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end);

public:
    SinglyLinkedList(sf::Font& font);
    ~SinglyLinkedList();

    void insertNode(int val); 
    void deleteNode(int val); 
    void startSearch(int val);
    void resetColors();

    void updatePosition(float deltaTime);
    void updateSearching(float deltaTime);
    void draw(sf::RenderWindow& window);
};