#include "SinglyLinkedList.h"
#include <cstdlib>
#include <ctime>
#include <fstream>

SinglyLinkedList::SinglyLinkedList(sf::Font& font) : head(nullptr), idCounter(0), font(font), currentStep(0), timer(0), delay(0.8f), isPaused(false) {
    std::srand(std::time(0));

    // Khởi tạo giao diện Box Code
    codeBox.setSize(sf::Vector2f(350.f, 250.f));
    codeBox.setFillColor(sf::Color(30, 30, 30, 200));
    codeBox.setOutlineThickness(2.f);
    codeBox.setOutlineColor(sf::Color::White);

    codeText.setFont(font);
    codeText.setCharacterSize(16);
    currentHighlight = -1;
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

// Chụp lại toàn bộ trạng thái toạ độ, màu sắc và Code
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
        "for(int i = 0; i < n; i++)",
        "  Insert(random_val);"
    };

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
    saveSnapshot(code, -1);
    currentStep = 0;
    applyStep(0);
}

void SinglyLinkedList::initFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return; // Thoát nếu không tìm thấy file
    
    clear();
    std::vector<std::string> code = {
        "Read from file: " + filename,
        "while(file >> val) {",
        "   Insert(val);",
        "}"
    };
    
    int val;
    while (file >> val) {
        LogicalNode* newNode = new LogicalNode(idCounter++, val);
        if (!head) head = newNode;
        else {
            LogicalNode* curr = head;
            while (curr->next) curr = curr->next;
            curr->next = newNode;
        }
    }
    file.close();
    saveSnapshot(code, 1);
    currentStep = 0;
    applyStep(0);
}

void SinglyLinkedList::insertNode(int val) {
    std::vector<std::string> code = {
        "Node* newNode = new Node(v);",
        "if (head == null) head = newNode;",
        "else {",
        "  Node* curr = head;",
        "  while(curr->next != null)",
        "    curr = curr->next;",
        "  curr->next = newNode;",
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
        "while (curr != null) {",
        "  if (curr->value == val) return true;",
        "  curr = curr->next;",
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
        "Node* curr = head, *prev = null;",
        "while (curr != null && curr->value != val) {",
        "  prev = curr;",
        "  curr = curr->next;",
        "}",
        "if (curr != null) {",
        "  if (prev == null) head = head->next;",
        "  else prev->next = curr->next;",
        "  delete curr;",
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

    saveSnapshot(code, 1); // Thoát vòng lặp

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

// Vẽ UI Box hiển thị Pseudo-code
void SinglyLinkedList::drawCode(sf::RenderWindow& window) {
    if (!currentCode.empty()) {
        // Đặt Box góc phải dưới
        float boxX = window.getSize().x - 380.f;
        float boxY = window.getSize().y - 280.f;
        codeBox.setPosition(boxX, boxY);
        window.draw(codeBox);

        for (size_t i = 0; i < currentCode.size(); ++i) {
            codeText.setString(currentCode[i]);
            codeText.setPosition(boxX + 15.f, boxY + 15.f + i * 22.f);
            
            if (static_cast<int>(i) == currentHighlight) {
                codeText.setFillColor(sf::Color::Yellow); // Highlight màu vàng
            } else {
                codeText.setFillColor(sf::Color::White);
            }
            window.draw(codeText);
        }
    }
}