#include "SinglyLinkedList.h"
#include <cstdlib>
#include <ctime>
#include <fstream>

SinglyLinkedList::SinglyLinkedList(sf::Font& font) : head(nullptr), idCounter(0), font(font), currentStep(0), timer(0), delay(0.8f), isPaused(false) {
    std::srand(std::time(0));

    codeBox.setFillColor(sf::Color(253, 246, 227)); // Màu be sáng
    codeBox.setOutlineThickness(1.f);
    codeBox.setOutlineColor(sf::Color::Black);

    codeText.setFont(font);
    codeText.setCharacterSize(15);
    codeText.setFillColor(sf::Color::Black);
    currentHighlight = -1;
}

SinglyLinkedList::~SinglyLinkedList() { clear(); }

void SinglyLinkedList::clear() {
    clearLogical();
    for (auto& pair : visualNodes) delete pair.second;
    visualNodes.clear();
    snapshots.clear();
    currentCode.clear(); 
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

void SinglyLinkedList::saveSnapshot(const std::vector<std::string>& code, int line) {
    SLLStepSnapshot snap;
    snap.codeLines = code;
    snap.highlightLine = line;

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

    currentCode = snapshots[stepIndex].codeLines;
    currentHighlight = snapshots[stepIndex].highlightLine;

    for (const auto& info : snapshots[stepIndex].nodes) {
        if (visualNodes.find(info.id) == visualNodes.end()) {
            visualNodes[info.id] = new ListNode(info.id, info.value, font);
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
    std::vector<std::string> code = {
        "Initialize list with random values",
        "for (int i = 0; i < n; i++)",
        "    InsertAtTail(random_val);"
    };
    saveSnapshot(code, 0);

    for (int i = 0; i < n; i++) {
        saveSnapshot(code, 1);
        int val = std::rand() % 100 + 1;
        LogicalNode* newNode = new LogicalNode(idCounter++, val);
        newNode->color = sf::Color(50, 205, 50);

        if (!head) head = newNode;
        else {
            LogicalNode* curr = head;
            while (curr->next) curr = curr->next;
            curr->next = newNode;
        }
        saveSnapshot(code, 2);
        resetColors();
    }
    saveSnapshot(code, -1);
    currentStep = 0;
    applyStep(0);
}

void SinglyLinkedList::initFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return; 
    
    clear();
    std::vector<std::string> code = {
        "Read from file: " + filename,
        "while(file >> val) {",
        "    InsertAtTail(val);",
        "}"
    };
    saveSnapshot(code, 0);
    
    int val;
    while (file >> val) {
        saveSnapshot(code, 1);
        LogicalNode* newNode = new LogicalNode(idCounter++, val);
        newNode->color = sf::Color(50, 205, 50);

        if (!head) head = newNode;
        else {
            LogicalNode* curr = head;
            while (curr->next) curr = curr->next;
            curr->next = newNode;
        }
        saveSnapshot(code, 2);
        resetColors();
    }
    file.close();
    saveSnapshot(code, -1);
    currentStep = 0;
    applyStep(0);
}

void SinglyLinkedList::insertNode(int val) {
    std::vector<std::string> code = {
        "Node* newNode = new Node(v);",
        "if (head == NULL) head = newNode;",
        "else {",
        "    Node* curr = head;",
        "    while (curr->next != NULL)",
        "        curr = curr->next;",
        "    curr->next = newNode;",
        "}"
    };
    snapshots.clear(); currentStep = 0;
    resetColors(); saveSnapshot(code, -1); 

    LogicalNode* newNode = new LogicalNode(idCounter++, val);
    newNode->color = sf::Color(50, 205, 50); 
    saveSnapshot(code, 0);

    if (!head) {
        head = newNode;
        saveSnapshot(code, 1);
    } else {
        saveSnapshot(code, 2);
        LogicalNode* curr = head;
        saveSnapshot(code, 3);

        while (curr->next) {
            curr->color = sf::Color(255, 165, 0); 
            saveSnapshot(code, 4);

            curr->color = sf::Color(169, 169, 169); 
            curr = curr->next;
            saveSnapshot(code, 5);
        }
        curr->color = sf::Color(255, 165, 0);
        saveSnapshot(code, 4); 
        
        curr->next = newNode;
        saveSnapshot(code, 6);
    }
    
    saveSnapshot(code, 7);
    resetColors();
    saveSnapshot(code, -1); 

    applyStep(0);
    isPaused = false;
}

void SinglyLinkedList::startSearch(int val) {
    if (!head) return;
    std::vector<std::string> code = {
        "Node* curr = head;",
        "while (curr != NULL) {",
        "    if (curr->value == val) return true;",
        "    curr = curr->next;",
        "}",
        "return false;"
    };

    snapshots.clear(); currentStep = 0;
    resetColors(); saveSnapshot(code, -1);

    LogicalNode* curr = head;
    saveSnapshot(code, 0);

    while (curr) {
        saveSnapshot(code, 1);
        if (curr->value == val) {
            curr->color = sf::Color(50, 205, 50); 
            saveSnapshot(code, 2);
            break; 
        } else {
            curr->color = sf::Color(255, 165, 0); 
            saveSnapshot(code, 2); 
            curr->color = sf::Color(169, 169, 169); 
            curr = curr->next;
            saveSnapshot(code, 3);
        }
    }

    if (!curr) saveSnapshot(code, 5); 

    applyStep(0);
    isPaused = false;
}

void SinglyLinkedList::startDelete(int val) {
    if (!head) return;
    std::vector<std::string> code = {
        "Node* curr = head, *prev = NULL;",
        "while (curr != NULL && curr->value != val) {",
        "    prev = curr;",
        "    curr = curr->next;",
        "}",
        "if (curr != NULL) {",
        "    if (prev == NULL) head = head->next;",
        "    else prev->next = curr->next;",
        "    delete curr;",
        "}"
    };

    snapshots.clear(); currentStep = 0;
    resetColors(); saveSnapshot(code, -1);

    LogicalNode* curr = head;
    LogicalNode* prev = nullptr;
    saveSnapshot(code, 0);

    while (curr && curr->value != val) {
        curr->color = sf::Color(255, 165, 0);
        saveSnapshot(code, 1);
        
        prev = curr;
        saveSnapshot(code, 2);

        curr->color = sf::Color(169, 169, 169);
        curr = curr->next;
        saveSnapshot(code, 3);
    }

    saveSnapshot(code, 1); 

    if (curr) { 
        saveSnapshot(code, 5);
        curr->color = sf::Color(139, 0, 0); 
        
        if (prev == nullptr) {
            head = head->next;
            saveSnapshot(code, 6);
        } else {
            prev->next = curr->next;
            saveSnapshot(code, 7);
        }
        
        delete curr; 
        saveSnapshot(code, 8); 
    }

    resetColors();
    saveSnapshot(code, -1); 
    applyStep(0);
    isPaused = false;
}

// THÊM CODE NÀY VÀO TRONG FILE SinglyLinkedList.cpp
void SinglyLinkedList::insertAtHead(int val) {
    std::vector<std::string> code = {
        "Node* newNode = new Node(v);",
        "newNode->next = head;",
        "head = newNode;"
    };
    snapshots.clear(); currentStep = 0;
    resetColors(); saveSnapshot(code, -1); 

    // Bước 0: Khởi tạo Node mới
    LogicalNode* newNode = new LogicalNode(idCounter++, val);
    newNode->color = sf::Color(50, 205, 50); // Màu xanh lá mới
    saveSnapshot(code, 0);

    // Bước 1: Trỏ next của newNode vào head cũ
    newNode->next = head;
    saveSnapshot(code, 1);

    // Bước 2: Cập nhật head
    head = newNode;
    saveSnapshot(code, 2);

    // Kết thúc
    resetColors();
    saveSnapshot(code, -1); 

    applyStep(0);
    isPaused = false;
}

void SinglyLinkedList::insertAtIndex(int val, int index) {
    if (index <= 0) { insertAtHead(val); return; } // Nếu index = 0 thì tương tự Ins Head

    std::vector<std::string> code = {
        "Node* newNode = new Node(v);",
        "Node* curr = head;",
        "for (int i = 0; i < index - 1 && curr != NULL; i++)",
        "    curr = curr->next;",
        "if (curr == NULL) return;",
        "newNode->next = curr->next;",
        "curr->next = newNode;"
    };
    snapshots.clear(); currentStep = 0;
    resetColors(); saveSnapshot(code, -1);

    LogicalNode* newNode = new LogicalNode(idCounter++, val);
    newNode->color = sf::Color(50, 205, 50);
    saveSnapshot(code, 0);

    LogicalNode* curr = head;
    saveSnapshot(code, 1);

    // Lặp tìm vị trí index - 1
    for (int i = 0; i < index - 1 && curr != nullptr; i++) {
        curr->color = sf::Color(255, 165, 0); // Đổi cam
        saveSnapshot(code, 2);
        
        curr->color = sf::Color(169, 169, 169); // Node đã qua màu xám
        curr = curr->next;
        saveSnapshot(code, 3);
    }

    if (curr == nullptr) {
        // Out of bounds (Index lớn hơn size list), huỷ newNode
        saveSnapshot(code, 4);
        delete newNode;
        applyStep(0);
        isPaused = false;
        return;
    }

    curr->color = sf::Color(255, 165, 0); // Highlight node tại index-1
    saveSnapshot(code, 2);

    // Trỏ next của newNode
    newNode->next = curr->next;
    saveSnapshot(code, 5);

    // Nối Node cũ vào newNode
    curr->next = newNode;
    saveSnapshot(code, 6);

    resetColors();
    saveSnapshot(code, -1);
    applyStep(0);
    isPaused = false;
}

void SinglyLinkedList::stepForward() {
    if (snapshots.empty()) return;
    
    // Tự động Pause thuật toán và reset lại bộ đếm thời gian
    isPaused = true; 
    timer = 0.0f;    
    
    if (currentStep < snapshots.size() - 1) {
        currentStep++;
        applyStep(currentStep);
    }
}

void SinglyLinkedList::stepBackward() {
    if (snapshots.empty()) return;
    
    // Tự động Pause thuật toán và reset lại bộ đếm thời gian
    isPaused = true; 
    timer = 0.0f;    
    
    if (currentStep > 0) {
        currentStep--;
        applyStep(currentStep);
    }
}

// THÊM CODE NÀY VÀO TRONG FILE SinglyLinkedList.cpp
void SinglyLinkedList::skipToFirstStep() {
    if (snapshots.empty()) return;
    
    isPaused = true;  // Tự động Pause thuật toán
    timer = 0.0f;     // Reset bộ đếm thời gian
    
    currentStep = 0;  // Nhảy về bước đầu tiên
    applyStep(currentStep);
}

void SinglyLinkedList::skipToLastStep() {
    if (snapshots.empty()) return;
    
    isPaused = true;  // Tự động Pause thuật toán
    timer = 0.0f;     // Reset bộ đếm thời gian
    
    currentStep = snapshots.size() - 1; // Nhảy tới bước cuối cùng
    applyStep(currentStep);
}

void SinglyLinkedList::updateAnimation(float deltaTime) {
    if (snapshots.empty() || isPaused) return;
    timer += deltaTime;
    if (timer >= delay) {
        timer = 0.0f;
        if (currentStep < snapshots.size() - 1) {
            // Chuyển bước trực tiếp thay vì gọi hàm stepForward() 
            // để tránh kích hoạt tính năng tự động Pause ở trên
            currentStep++;
            applyStep(currentStep);
        }
        else {
            isPaused = true; // Đạt đến bước cuối cùng thì dừng lại
        }
    }
}

void SinglyLinkedList::increaseSpeed() { delay = std::max(0.1f, delay - 0.2f); }
void SinglyLinkedList::decreaseSpeed() { delay = std::min(2.0f, delay + 0.2f); }


void SinglyLinkedList::updatePosition(float deltaTime) {
    for (auto& pair : visualNodes) pair.second->update(deltaTime);
}

void SinglyLinkedList::draw(sf::RenderWindow& window) {
    for (auto& pair : visualNodes) pair.second->drawArrow(window, visualNodes);
    for (auto& pair : visualNodes) pair.second->draw(window);
}



void SinglyLinkedList::drawCode(sf::RenderWindow& window) {
    if (currentCode.empty()) return;

    float boxWidth = 380.f;
    float lineHeight = 24.f;
    float boxHeight = currentCode.size() * lineHeight + 15.f; 
    
    // Đã thay đổi: Dùng View ảo (getDefaultView) thay vì Kích thước Cửa Sổ vật lý
    // Lệnh này đảm bảo CodeBox luôn bám đúng vào góc phải bất kể bạn Maximize hay Resize
    sf::Vector2f viewSize = window.getDefaultView().getSize();
    float boxX = viewSize.x - boxWidth - 30.f;
    float boxY = viewSize.y - boxHeight - 30.f;

    codeBox.setSize(sf::Vector2f(boxWidth, boxHeight));
    codeBox.setPosition(boxX, boxY);
    window.draw(codeBox); 

    for (size_t i = 0; i < currentCode.size(); ++i) {
        float lineY = boxY + 7.f + i * lineHeight;

        if (static_cast<int>(i) == currentHighlight) {
            sf::RectangleShape highlightRect;
            highlightRect.setSize(sf::Vector2f(boxWidth, lineHeight));
            highlightRect.setPosition(boxX, lineY);
            highlightRect.setFillColor(sf::Color(255, 228, 181)); 
            window.draw(highlightRect);
        }

        codeText.setString(currentCode[i]);
        codeText.setPosition(boxX + 15.f, lineY + 2.f);
        window.draw(codeText);
    }
}