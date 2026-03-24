#include "SinglyLinkedList.h"
#include <cstdlib> // for rand()
#include <ctime>   // for time()

SinglyLinkedList::SinglyLinkedList(sf::Font& font) : font(font) {
    head = nullptr;
    // ... init existing states ...
    timer = 0.f;
    delay = 0.8f; 
    std::srand(std::time(0)); // seed random
}

SinglyLinkedList::~SinglyLinkedList() {
    clear();
}

void SinglyLinkedList::clear() {
    ListNode* curr = head;
    while (curr != nullptr) {
        ListNode* nextNode = curr->next;
        delete curr;
        curr = nextNode;
    }
    head = nullptr;
}

void SinglyLinkedList::initList(int n) {
    if (n <= 0) return;
    clear();
    resetColors();

    for (int i = 0; i < n; ++i) {
        int val = std::rand() % 100 + 1; // 1 to 100
        insertNodeNoAnimation(val);
    }
}
void SinglyLinkedList::updateLayout() {
    ListNode* curr = head;
    int index = 0;
    while (curr != nullptr) {
        // Cập nhật vị trí đích (targetPos) cho từng node để nó tự động di chuyển tới
        curr->targetPos = sf::Vector2f(startX + (index * nodeSpacing), startY);
        curr = curr->next;
        index++;
    }
}
void SinglyLinkedList::insertNodeNoAnimation(int val) {
    ListNode* newNode = new ListNode(val, font);
    if (head == nullptr) {
        head = newNode;
    } else {
        ListNode* curr = head;
        while (curr->next != nullptr) curr = curr->next;
        curr->next = newNode;
    }
    resetColors();
    updateLayout();
    // Override falling animation
    ListNode* curr = head;
    int index = 0;
    while (curr != nullptr) {
        curr->currentPos = sf::Vector2f(startX + (index * nodeSpacing), startY);
        curr = curr->next;
        index++;
    }
}

void SinglyLinkedList::insertNode(int val) {
    ListNode* newNode = new ListNode(val, font);
    if (head == nullptr) head = newNode;
    else {
        ListNode* curr = head;
        int count = 1;
        while (curr->next != nullptr) { curr = curr->next; count++; }
        curr->next = newNode;
        newNode->currentPos = sf::Vector2f(startX + count * nodeSpacing, startY - 200.f); // Falling from top
    }
    resetColors();
    updateLayout(); 
}

void SinglyLinkedList::startDelete(int val) {
    if (head == nullptr) return;
    resetColors();
    isDeleting = true;
    deleteValue = val;
    currentDeleteNode = head;
    previousDeleteNode = nullptr;
    isConfirmingDeletion = false;
    animationStepCount = 0;
    timer = 0.0f;
    currentDeleteNode->setColor(sf::Color(255, 165, 0)); // Start with head as Orange (checking)
}

void SinglyLinkedList::startSearch(int val) {
    if (head == nullptr) return;
    resetColors();
    isSearching = true;
    searchValue = val;
    currentSearchNode = head;
    animationStepCount = 0;
    timer = 0.0f;
    currentSearchNode->setColor(sf::Color(255, 165, 0)); 
}

void SinglyLinkedList::resetColors() {
    isSearching = false;
    isDeleting = false;
    isConfirmingDeletion = false;
    ListNode* curr = head;
    while (curr != nullptr) {
        curr->setColor(sf::Color(70, 130, 180)); // Steel blue
        curr = curr->next;
    }
}

void SinglyLinkedList::resetColorsToStep() {
    ListNode* curr = head;
    size_t step = 0;
    while(curr != nullptr && step < animationStepCount) {
        if (isDeleting) curr->setColor(sf::Color::Red); 
        else if (isSearching) curr->setColor(sf::Color(169, 169, 169)); // <-- Fix màu ở đây
        curr = curr->next;
        step++;
    }
    if (isSearching) {
        if (previousSearchNode) previousSearchNode->setColor(sf::Color(255, 165, 0)); 
        if (currentSearchNode) currentSearchNode->setColor(sf::Color(169, 169, 169)); // <-- Fix màu ở đây
    } else if (isDeleting) {
        if (previousDeleteNode) previousDeleteNode->setColor(sf::Color(255, 165, 0));
        if (currentDeleteNode) currentDeleteNode->setColor(sf::Color::Red);
    }
}

ListNode* SinglyLinkedList::getNodeAt(ListNode* h, size_t index) {
    ListNode* curr = h;
    for (size_t i = 0; i < index && curr != nullptr; ++i) curr = curr->next;
    return curr;
}

// Logic separation for step commands
void SinglyLinkedList::handleSearchStep() {
    if (currentSearchNode == nullptr) { isSearching = false; resetColors(); return; }
    
    animationStepCount++;
    if (currentSearchNode->value == searchValue) {
        currentSearchNode->setColor(sf::Color(50, 205, 50)); 
        isSearching = false;
        return;
    }
    currentSearchNode->setColor(sf::Color(169, 169, 169)); 
    previousSearchNode = currentSearchNode; // <-- LƯU LẠI NODE TRƯỚC ĐÓ Ở ĐÂY
    currentSearchNode = currentSearchNode->next;
    
    if (currentSearchNode != nullptr) currentSearchNode->setColor(sf::Color(255, 165, 0)); 
    else { isSearching = false; }
}

void SinglyLinkedList::handleSearchBack() {
    if (animationStepCount == 0) return;
    animationStepCount--;
    currentSearchNode = getNodeAt(head, animationStepCount);
    // Tính lại previousSearchNode khi lui về
    previousSearchNode = (animationStepCount > 0) ? getNodeAt(head, animationStepCount - 1) : nullptr;
    resetColorsToStep(); 
}

void SinglyLinkedList::handleDeleteStep() {
    if (currentDeleteNode == nullptr) { isDeleting = false; resetColors(); return; }
    
    animationStepCount++;
    if (currentDeleteNode->value == deleteValue) {
        currentDeleteNode->setColor(sf::Color::Red); 
        isDeleting = false;
        isConfirmingDeletion = true; // Enter deletion confirmation phase
        timer = 0.0f; // start timer for confirm pause
        return;
    }
    currentDeleteNode->setColor(sf::Color::Red); // Visited
    previousDeleteNode = currentDeleteNode;
    currentDeleteNode = currentDeleteNode->next;
    if (currentDeleteNode != nullptr) currentDeleteNode->setColor(sf::Color(255, 165, 0)); // Checking
    else { isDeleting = false; }
}

void SinglyLinkedList::handleDeleteBack() {
    if (isConfirmingDeletion) { isConfirmingDeletion = false; isDeleting = true; } // reverse Phase 2
    if (animationStepCount == 0) return;
    animationStepCount--;
    // Recalculate
    currentDeleteNode = getNodeAt(head, animationStepCount);
    previousDeleteNode = (animationStepCount > 0) ? getNodeAt(head, animationStepCount - 1) : nullptr;
    resetColorsToStep();
}

void SinglyLinkedList::stepForward() {
    if (isSearching) handleSearchStep();
    else if (isDeleting) handleDeleteStep();
    else if (isConfirmingDeletion) {
        // Step forward during confirm: delete the node immediately
        ListNode* nodeToDelete = currentDeleteNode;
        if (previousDeleteNode == nullptr) head = head->next;
        else previousDeleteNode->next = currentDeleteNode->next;
        delete nodeToDelete;
        currentDeleteNode = nullptr;
        isConfirmingDeletion = false;
        updateLayout();
        resetColors();
    }
}

void SinglyLinkedList::stepBackward() {
    if (isSearching) handleSearchBack();
    else if (isDeleting || isConfirmingDeletion) handleDeleteBack();
}

void SinglyLinkedList::updatePosition(float deltaTime) {
    ListNode* curr = head;
    while (curr != nullptr) { curr->update(deltaTime); curr = curr->next; }
}

void SinglyLinkedList::updateAnimation(float deltaTime) {
    if (isPaused) return;

    // Search Animation
    if (isSearching) {
        timer += deltaTime;
        if (timer >= delay) { timer = 0.0f; handleSearchStep(); }
    }
    
    // Delete Animation
    else if (isDeleting) {
        timer += deltaTime;
        if (timer >= delay) { timer = 0.0f; handleDeleteStep(); }
    } 
    // Delete Confirmation Phase (Phase 2): Target found, highlight red for a while
    else if (isConfirmingDeletion) {
        timer += deltaTime;
        if (timer >= delay) {
            timer = 0.0f;
            // Finally delete the node
            ListNode* nodeToDelete = currentDeleteNode;
            if (previousDeleteNode == nullptr) head = head->next; // delete head
            else previousDeleteNode->next = currentDeleteNode->next; // link over
            delete nodeToDelete;
            currentDeleteNode = nullptr; // important
            isConfirmingDeletion = false; // exit Phase 2
            updateLayout();
            resetColors();
        }
    }
}

// ... unchanged drawing functions drawArrow(), draw() ...

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