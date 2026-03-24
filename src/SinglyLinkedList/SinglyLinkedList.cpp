#include "SinglyLinkedList.h"
#include <cstdlib> 
#include <ctime>   
#include <cmath>

SinglyLinkedList::SinglyLinkedList(sf::Font& font) : font(font) {
    head = nullptr;
    timer = 0.f;
    delay = 0.8f; 
    std::srand(std::time(0)); 
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
    for (int i = 0; i < n; ++i) {
        int val = std::rand() % 100 + 1; 
        insertNodeNoAnimation(val);
    }
    resetColors();
}

void SinglyLinkedList::insertNodeNoAnimation(int val) {
    ListNode* newNode = new ListNode(val, font);
    if (head == nullptr) head = newNode;
    else {
        ListNode* curr = head;
        while (curr->next != nullptr) curr = curr->next;
        curr->next = newNode;
    }
    updateLayout();
    ListNode* curr = head;
    int index = 0;
    while (curr != nullptr) {
        curr->currentPos = sf::Vector2f(startX + (index * nodeSpacing), startY);
        curr->targetPos = curr->currentPos;
        curr = curr->next;
        index++;
    }
    resetColors();
}

void SinglyLinkedList::insertNode(int val) {
    ListNode* newNode = new ListNode(val, font);
    if (head == nullptr) head = newNode;
    else {
        ListNode* curr = head;
        int count = 1;
        while (curr->next != nullptr) { curr = curr->next; count++; }
        curr->next = newNode;
        newNode->currentPos = sf::Vector2f(startX + count * nodeSpacing, startY - 200.f); 
    }
    updateLayout(); 
    resetColors();
}

// ---------------------------------------------------------
// KIẾN TRÚC PLAYBACK MỚI (CHỐNG LỖI NHẢY LUNG TUNG)
// ---------------------------------------------------------

void SinglyLinkedList::startSearch(int val) {
    if (head == nullptr) return;
    currentMode = AnimMode::SEARCH;
    targetValue = val;
    currentStep = 1; // Bắt đầu ở node đầu tiên
    timer = 0.0f;
    nodeWasFound = false;

    // Tính toán trước tổng số bước cần đi
    totalSteps = 0;
    ListNode* curr = head;
    while(curr) {
        totalSteps++;
        if (curr->value == val) { nodeWasFound = true; break; }
        curr = curr->next;
    }
    applyColorsByStep();
}

void SinglyLinkedList::startDelete(int val) {
    if (head == nullptr) return;
    currentMode = AnimMode::DELETE;
    targetValue = val;
    currentStep = 1; 
    timer = 0.0f;
    nodeWasFound = false;

    totalSteps = 0;
    ListNode* curr = head;
    while(curr) {
        totalSteps++;
        if (curr->value == val) { nodeWasFound = true; break; }
        curr = curr->next;
    }
    applyColorsByStep();
}

void SinglyLinkedList::resetColors() {
    currentMode = AnimMode::NONE; // Tắt chế độ hoạt ảnh
    applyColorsByStep(); // Áp dụng => tất cả về xanh Blue
}

// Hàm trái tim của Playback: Tô màu lại chính xác dựa vào số currentStep
void SinglyLinkedList::applyColorsByStep() {
    // 1. Reset toàn bộ danh sách về xanh Default trước
    ListNode* curr = head;
    while (curr) {
        curr->setColor(sf::Color(70, 130, 180)); 
        curr = curr->next;
    }

    // Nếu bước 0 (về vạch xuất phát) thì dừng ở màu xanh
    if (currentStep == 0 || currentMode == AnimMode::NONE) return;

    // 2. Bắt đầu tô màu theo số bước hiện tại
    curr = head;
    size_t stepCounter = 1;

    while (curr != nullptr && stepCounter <= currentStep) {
        if (stepCounter == currentStep) {
            // ĐÂY LÀ NODE ĐANG ĐƯỢC XÉT
            if (currentMode == AnimMode::SEARCH) {
                if (curr->value == targetValue) curr->setColor(sf::Color(50, 205, 50)); // Tìm thấy -> Xanh lá
                else curr->setColor(sf::Color(255, 165, 0)); // Đang quét -> Cam
            } else if (currentMode == AnimMode::DELETE) {
                if (curr->value == targetValue) curr->setColor(sf::Color(139, 0, 0)); // Sắp bị xoá -> Đỏ Đậm
                else curr->setColor(sf::Color(255, 165, 0)); // Đang quét -> Cam
            }
        } else {
            // CÁC NODE ĐÃ ĐI QUA (VISITED)
            if (currentMode == AnimMode::SEARCH) curr->setColor(sf::Color(169, 169, 169)); // Xám
            else if (currentMode == AnimMode::DELETE) curr->setColor(sf::Color(255, 0, 0)); // Đỏ
        }
        curr = curr->next;
        stepCounter++;
    }
}

// Xoá node vật lý thực sự ra khỏi danh sách liên kết
void SinglyLinkedList::performPhysicalDelete() {
    if (head == nullptr) return;
    if (head->value == targetValue) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    } else {
        ListNode* curr = head;
        while (curr->next != nullptr && curr->next->value != targetValue) curr = curr->next;
        if (curr->next != nullptr) {
            ListNode* temp = curr->next;
            curr->next = temp->next;
            delete temp;
        }
    }
    updateLayout();
}

void SinglyLinkedList::stepForward() {
    if (currentMode == AnimMode::NONE) return;

    if (currentStep < totalSteps) {
        currentStep++; // Tiến 1 bước
        applyColorsByStep();
    } 
    // SỬA Ở ĐÂY: Nếu người dùng bấm TỚI (>) khi đang ở bước cuối cùng -> Kết thúc quy trình
    else if (currentStep == totalSteps) {
        if (currentMode == AnimMode::DELETE && nodeWasFound) {
            performPhysicalDelete(); // Thực sự xoá node khỏi danh sách
        }
        currentMode = AnimMode::NONE; // Reset trạng thái
        applyColorsByStep(); // Trả về xanh
    }
}

void SinglyLinkedList::stepBackward() {
    if (currentMode == AnimMode::NONE) return;
    
    // Lui về tối đa là 0 (Bước 0 = Chờ, tất cả xanh lam)
    if (currentStep > 0) {
        currentStep--; 
        applyColorsByStep();
    }
}
void SinglyLinkedList::updateAnimation(float deltaTime) {
    if (isPaused || currentMode == AnimMode::NONE) return;

    timer += deltaTime;
    if (timer >= delay) {
        timer = 0.0f;
        
        // SỬA Ở ĐÂY: Nếu đang chạy tự động mà chạm đích -> Tự động Pause để người dùng xem/lùi lại
        if (currentStep == totalSteps) {
            isPaused = true; 
        } else {
            stepForward();
        }
    }
}
// ---------------------------------------------------------
// DRAWING VÀ CẬP NHẬT GIAO DIỆN (Giữ nguyên)
// ---------------------------------------------------------
void SinglyLinkedList::updateLayout() {
    ListNode* curr = head;
    int index = 0;
    while (curr != nullptr) {
        curr->targetPos = sf::Vector2f(startX + (index * nodeSpacing), startY);
        curr = curr->next;
        index++;
    }
}

void SinglyLinkedList::updatePosition(float deltaTime) {
    ListNode* curr = head;
    while (curr != nullptr) { curr->update(deltaTime); curr = curr->next; }
}

void SinglyLinkedList::drawArrow(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end) {
    sf::Vertex line[] = { sf::Vertex(start), sf::Vertex(end) };
    window.draw(line, 2, sf::Lines);
    
    float angle = atan2(end.y - start.y, end.x - start.x);
    sf::CircleShape arrowHead(5, 3);
    arrowHead.setFillColor(sf::Color::White);
    arrowHead.setOrigin(5, 5);
    arrowHead.setPosition(end);
    arrowHead.setRotation(angle * 180 / 3.14159f + 90);
    window.draw(arrowHead);
}

void SinglyLinkedList::draw(sf::RenderWindow& window) {
    ListNode* curr = head;
    while (curr != nullptr) {
        if (curr->next != nullptr) {
            sf::Vector2f start(curr->currentPos.x + 30, curr->currentPos.y); 
            sf::Vector2f end(curr->next->currentPos.x - 30, curr->next->currentPos.y);
            drawArrow(window, start, end);
        }
        curr->draw(window);
        curr = curr->next;
    }
}

// Thêm vào cuối file SinglyLinkedList.cpp
void SinglyLinkedList::increaseSpeed() {
    delay -= 0.2f; // Giảm thời gian chờ
    if (delay < 0.2f) delay = 0.2f; // Giới hạn tốc độ max
}

void SinglyLinkedList::decreaseSpeed() {
    delay += 0.2f; // Tăng thời gian chờ
    if (delay > 2.0f) delay = 2.0f; // Giới hạn tốc độ min
}