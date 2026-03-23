#pragma once
#include "ListNode.h"

enum class SLLState {
    Idle,
    Initializing,
    Searching,
    Inserting,
    Deleting
};

class SinglyLinkedList{
    private:
        ListNode* head;
        sf::Font font;
        // bien phuc vu animation

        SLLState currentstate;
        ListNode* currentNode;
        int NodeRadius;
        int targetvalue;
        float timer;

    // ham phhu tro
    void initVariable(sf::Font &font);
    void drawArrow(sf::RenderWindow &window, sf::Vector2f start, sf::Vector2f end);

    public:
        SinglyLinkedList(sf::Font& font);
        ~SinglyLinkedList();
        // quan ly du lieu
        void clear();
        // 2. Cac ham chuc nang chinh
        void startSearch(int val);
        void updateSearching(float deltaTime);
        void updatePosition(float deltaTime);
        void draw(sf::RenderWindow& window) ;
};


