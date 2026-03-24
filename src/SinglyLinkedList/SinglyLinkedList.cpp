#include "SinglyLinkedList.h"
#include <cstdlib>
#include <ctime>

SinglyLinkedList::SinglyLinkedList(sf::Font& font) : head(nullptr), idCounter(0), font(font), currentStep(0), timer(0), delay(0.8f), isPaused(false) {
    std::srand(std::time(0));
}

SinglyLinkedList::~SinglyLinkedList() { clear(); }

void SinglyLinkedList::clear() {
    clearLogical();
    for (auto& pair : visualNodes) delete pair.second;
    visualNodes.clear();
    snapshots.clear();
}

void SinglyLinkedList::clearLogical() {
    LogicalNode* curr = head;
    while (curr != nullptr) {
        LogicalNode* nextNode = curr->next;
        delete curr;
        curr = nextNode;
    }
    head = nullptr;
}

void SinglyLinkedList::resetColors() {
    LogicalNode* curr = head;
    while (curr) {
        curr->color = sf::Color(70, 130, 180);
        curr = curr->next;
    }
}

// Chụp lại toàn bộ trạng thái toạ độ, màu sắc hiện tại
void SinglyLinkedList::saveSnapshot() {
    SLLStepSnapshot snap;
    LogicalNode* curr = head;
    int index = 0;
    while (curr) {
        SLLNodeInfo info;
        info.id = curr->id;
        info.value = curr->value;
        info.pos = sf::Vector2f(startX + index * nodeSpacing, startY);
        info.color = curr->color;
        info.nextId = curr->next ? curr->next->id : -1;
        
        snap.nodes.push_back(info);
        curr = curr->next;
        index++;
    }
    snapshots.push_back(snap);
}

void SinglyLinkedList::applyStep(size_t stepIndex) {
    if (stepIndex >= snapshots.size()) return;
    for (auto& pair : visualNodes) pair.second->isActive = false;

    for (const auto& info : snapshots[stepIndex].nodes) {
        if (visualNodes.find(info.id) == visualNodes.end()) {
            visualNodes[info.id] = new ListNode(info.id, info.value, font);
            // Spawn từ trên trời rớt xuống cho đẹp
            visualNodes[info.id]->currentPos = sf::Vector2f(info.pos.x, info.pos.y - 150.f); 
        }
        ListNode* vn = visualNodes[info.id];
        vn->isActive = true;
        vn->targetPos = info.pos;
        vn->targetColor = info.color;
        vn->nextId = info.nextId;
    }
}

void SinglyLinkedList::initList(int n) {
    clear();
    for (int i = 0; i < n; i++) {
        int val = std::rand() % 100 + 1;
        LogicalNode* newNode = new LogicalNode(idCounter++, val);
        if (!head) head = newNode;
        else {
            LogicalNode* curr = head;
            while (curr->next) curr = curr->next;
            curr->next = newNode;
        }
    }
    saveSnapshot(); // Snap đầu tiên
    currentStep = 0;
    applyStep(0);
}

void SinglyLinkedList::insertNode(int val) {
    snapshots.clear(); currentStep = 0;
    resetColors(); saveSnapshot(); // Bắt đầu

    LogicalNode* newNode = new LogicalNode(idCounter++, val);
    newNode->color = sf::Color(50, 205, 50); // Màu xanh lá khi mới thêm

    if (!head) head = newNode;
    else {
        LogicalNode* curr = head;
        while (curr->next) {
            curr->color = sf::Color(255, 165, 0); // Cam quét qua
            saveSnapshot();
            curr->color = sf::Color(169, 169, 169); // Xám (đã đi qua)
            curr = curr->next;
        }
        curr->color = sf::Color(255, 165, 0);
        saveSnapshot();
        curr->color = sf::Color(169, 169, 169);
        
        curr->next = newNode;
    }
    
    saveSnapshot(); // Chụp khoảnh khắc chèn
    resetColors();
    saveSnapshot(); // Chụp khoảnh khắc hoàn thành

    applyStep(0);
    isPaused = false;
}

void SinglyLinkedList::startSearch(int val) {
    if (!head) return;
    snapshots.clear(); currentStep = 0;
    resetColors(); saveSnapshot();

    LogicalNode* curr = head;
    while (curr) {
        if (curr->value == val) {
            curr->color = sf::Color(50, 205, 50); // Xanh lá - Tìm thấy
            saveSnapshot();
            break; // Tìm thấy thì dừng
        } else {
            curr->color = sf::Color(255, 165, 0); // Cam - Đang quét
            saveSnapshot();
            curr->color = sf::Color(169, 169, 169); // Xám - Đã qua
        }
        curr = curr->next;
    }

    if (!curr) saveSnapshot(); // Quét hết không thấy

    applyStep(0);
    isPaused = false;
}

void SinglyLinkedList::startDelete(int val) {
    if (!head) return;
    snapshots.clear(); currentStep = 0;
    resetColors(); saveSnapshot();

    LogicalNode* curr = head;
    LogicalNode* prev = nullptr;

    while (curr && curr->value != val) {
        curr->color = sf::Color(255, 165, 0);
        saveSnapshot();
        curr->color = sf::Color(169, 169, 169);
        prev = curr;
        curr = curr->next;
    }

    if (curr) { // Tìm thấy
        curr->color = sf::Color(139, 0, 0); // Đỏ - Chuẩn bị xoá
        saveSnapshot();
        
        if (prev == nullptr) head = head->next;
        else prev->next = curr->next;
        
        delete curr; // Xoá vật lý
        saveSnapshot(); // Chụp ngay sau khi xoá để thấy list khép lại
    }

    resetColors();
    saveSnapshot(); // Khôi phục xanh
    applyStep(0);
    isPaused = false;
}

void SinglyLinkedList::stepForward() {
    if (snapshots.empty()) return;
    if (currentStep < snapshots.size() - 1) {
        currentStep++;
        applyStep(currentStep);
    }
}

void SinglyLinkedList::stepBackward() {
    if (snapshots.empty()) return;
    if (currentStep > 0) {
        currentStep--;
        applyStep(currentStep);
    }
}

void SinglyLinkedList::increaseSpeed() { delay = std::max(0.1f, delay - 0.2f); }
void SinglyLinkedList::decreaseSpeed() { delay = std::min(2.0f, delay + 0.2f); }

void SinglyLinkedList::updateAnimation(float deltaTime) {
    if (snapshots.empty() || isPaused) return;
    timer += deltaTime;
    if (timer >= delay) {
        timer = 0.0f;
        if (currentStep < snapshots.size() - 1) stepForward();
        else isPaused = true;
    }
}

void SinglyLinkedList::updatePosition(float deltaTime) {
    for (auto& pair : visualNodes) pair.second->update(deltaTime);
}

void SinglyLinkedList::draw(sf::RenderWindow& window) {
    for (auto& pair : visualNodes) pair.second->drawArrow(window, visualNodes);
    for (auto& pair : visualNodes) pair.second->draw(window);
}