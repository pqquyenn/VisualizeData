#include "SinglyLinkedList.h"

SinglyLinkedList::SinglyLinkedList(sf::Font& font) : font(font) {
    head = nullptr;
    isSearching = false;
    searchValue = 0;
    currentSearchNode = nullptr;
    searchTimer = 0.f;
    searchDelay = 0.8f; // Thời gian đổi màu từng node (0.8 giây)
}

SinglyLinkedList::~SinglyLinkedList() {
    ListNode* curr = head;
    while (curr != nullptr) {
        ListNode* nextNode = curr->next;
        delete curr;
        curr = nextNode;
    }
}

void SinglyLinkedList::insertNode(int val) {
    ListNode* newNode = new ListNode(val, font);
    
    if (head == nullptr) {
        head = newNode;
        newNode->currentPos = sf::Vector2f(startX, startY - 200.f); // Rơi từ trên xuống
    } else {
        ListNode* curr = head;
        int count = 1;
        while (curr->next != nullptr) {
            curr = curr->next;
            count++;
        }
        curr->next = newNode;
        newNode->currentPos = sf::Vector2f(startX + count * nodeSpacing, startY - 200.f);
    }
    
    resetColors();
    updateLayout(); 
}

void SinglyLinkedList::deleteNode(int val) {
    if (head == nullptr) return;

    resetColors();
    if (head->value == val) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
        updateLayout();
        return;
    }

    ListNode* curr = head;
    while (curr->next != nullptr && curr->next->value != val) {
        curr = curr->next;
    }

    if (curr->next != nullptr) {
        ListNode* temp = curr->next;
        curr->next = curr->next->next;
        delete temp;
        updateLayout(); 
    }
}

void SinglyLinkedList::startSearch(int val) {
    if (head == nullptr) return;

    resetColors();
    isSearching = true;
    searchValue = val;
    currentSearchNode = head;
    searchTimer = 0.0f;
    
    currentSearchNode->setColor(sf::Color(255, 165, 0)); // Màu cam cho node đang xét
}

void SinglyLinkedList::resetColors() {
    isSearching = false;
    ListNode* curr = head;
    while (curr != nullptr) {
        curr->setColor(sf::Color(70, 130, 180)); // Trả về màu xanh dương mặc định
        curr = curr->next;
    }
}

void SinglyLinkedList::updateLayout() {
    ListNode* curr = head;
    int index = 0;
    while (curr != nullptr) {
        curr->setTargetPosition(sf::Vector2f(startX + (index * nodeSpacing), startY));
        curr = curr->next;
        index++;
    }
}

void SinglyLinkedList::updatePosition(float deltaTime) {
    ListNode* curr = head;
    while (curr != nullptr) {
        curr->update(deltaTime);
        curr = curr->next;
    }
}

void SinglyLinkedList::updateSearching(float deltaTime) {
    if (!isSearching || currentSearchNode == nullptr) return;

    searchTimer += deltaTime;
    if (searchTimer >= searchDelay) {
        searchTimer = 0.0f; 

        if (currentSearchNode->value == searchValue) {
            currentSearchNode->setColor(sf::Color(50, 205, 50)); // Tìm thấy -> Xanh lá
            isSearching = false; 
        } else {
            currentSearchNode->setColor(sf::Color(169, 169, 169)); // Đi qua -> Xám
            currentSearchNode = currentSearchNode->next;
            
            if (currentSearchNode != nullptr) {
                currentSearchNode->setColor(sf::Color(255, 165, 0)); // Tới node tiếp theo -> Cam
            } else {
                isSearching = false; // Hết danh sách không thấy
            }
        }
    }
}

void SinglyLinkedList::drawArrow(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end) {
    // Chỉ vẽ đường thẳng đơn giản làm sợi dây liên kết
    sf::Vertex line[] = {
        sf::Vertex(start, sf::Color::White),
        sf::Vertex(end, sf::Color::White)
    };
    window.draw(line, 2, sf::Lines);
}

void SinglyLinkedList::draw(sf::RenderWindow& window) {
    ListNode* curr = head;
    
    // Vẽ đường nối
    while (curr != nullptr && curr->next != nullptr) {
        drawArrow(window, curr->currentPos, curr->next->currentPos);
        curr = curr->next;
    }

    // Vẽ Node lên đè lên đường nối
    curr = head;
    while (curr != nullptr) {
        curr->draw(window);
        curr = curr->next;
    }
}