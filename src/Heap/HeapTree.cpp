#include "Heap/HeapTree.h"
#include <cmath>
#include <algorithm>
#include <ctime>

HeapTree::HeapTree(sf::Font& f) : font(f), currentStep(0), timer(0), delay(0.8f), isPaused(true), draggedNode(nullptr) {
    std::srand(std::time(0));
}

HeapTree::~HeapTree() { clear(); }

void HeapTree::clear(){
    heapData.clear();
    for (auto node : visualNodes) delete node;
    visualNodes.clear();
    snapshots.clear();
    currentStep = 0;
}

void HeapTree::calculateLayout(std::vector<HeapNodeInfo>& snapshotNodes) {
    snapshotNodes.clear();
    if (heapData.empty()) return;

    float startX = 600.0f; 
    float startY = 150.0f;
    float yGap = 80.0f;

    for (int i = 0; i < heapData.size(); ++i) {
        int level = std::log2(i + 1);
        int nodesInLevel = std::pow(2, level);
        int indexInLevel = i - (nodesInLevel - 1);
        
        float xGap = 250.0f / std::pow(2, level); 
        float x = startX - (nodesInLevel - 1) * xGap / 2.0f + indexInLevel * xGap;
        float y = startY + level * yGap;

        snapshotNodes.push_back({heapData[i], sf::Vector2f(x, y), sf::Color(70, 130, 180)}); 
    }
}

// 1. Cập nhật các hàm lưu Snapshot để nhận thêm tham số Code Line
void HeapTree::saveSnapshot(std::string op, int line) {
    HeapSnapshot snap;
    calculateLayout(snap.nodes);
    snap.operation = op;
    snap.activeLine = line;
    snapshots.push_back(snap);
}

void HeapTree::saveSnapshotCompare(int idx1, int idx2, std::string op, int line) {
    HeapSnapshot snap;
    calculateLayout(snap.nodes);
    if (idx1 < snap.nodes.size()) snap.nodes[idx1].color = sf::Color(255, 165, 0); 
    if (idx2 < snap.nodes.size()) snap.nodes[idx2].color = sf::Color(255, 165, 0);
    snap.operation = op;
    snap.activeLine = line;
    snapshots.push_back(snap);
}

void HeapTree::saveSnapshotSwap(int idx1, int idx2, std::string op, int line) {
    HeapSnapshot snap;
    calculateLayout(snap.nodes); 
    if (idx1 < snap.nodes.size()) snap.nodes[idx1].color = sf::Color(255, 165, 0);
    if (idx2 < snap.nodes.size()) snap.nodes[idx2].color = sf::Color(255, 165, 0);
    snap.operation = op;
    snap.activeLine = line;
    snapshots.push_back(snap);
}

// 2. Cài đặt hàm initFromFile
// 1. Sửa lại hàm initFromFile
void HeapTree::initFromFile(const std::vector<int>& data) {
    clear(); // Xóa dữ liệu cũ
    snapshots.clear(); // Xóa sạch lịch sử hoạt ảnh
    currentStep = 0;
    timer = 0;

    // Thay vì gọi buildHeap() một lần, ta lặp qua từng phần tử và lưu snapshot
    for (int val : data) {
        saveSnapshot("Insert", 0); // Dòng 0: heap.push_back(val)
        heapData.push_back(val);
        
        saveSnapshot("Insert", 1); // Dòng 1: index = heap.size() - 1
        
        if (heapData.size() > 1) {
            // Hàm heapifyUp đã có sẵn lệnh lưu snapshot bên trong
            heapifyUp(heapData.size() - 1);
        }
    }
    
    // Bắt đầu phát hoạt ảnh từ bước đầu tiên
    applyStep(0); 
    isPaused = false; 
}

// 2. Sửa lại hàm initTree
void HeapTree::initTree(int n) {
    clear(); // Xóa dữ liệu cũ
    snapshots.clear(); // Xóa sạch lịch sử hoạt ảnh
    currentStep = 0;
    timer = 0;

    // Tương tự, tạo random và chèn từng Node vào cây
    for (int i = 0; i < n; i++) {
        int val = std::rand() % 100 + 1;
        
        saveSnapshot("Insert", 0); // Dòng 0: heap.push_back(val)
        heapData.push_back(val);
        
        saveSnapshot("Insert", 1); // Dòng 1: index = heap.size() - 1
        
        if (heapData.size() > 1) {
            heapifyUp(heapData.size() - 1);
        }
    }
    
    // Bắt đầu phát hoạt ảnh từ bước đầu tiên
    applyStep(0); 
    isPaused = false; 
}
void HeapTree::buildHeap() {
    int n = heapData.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        int idx = i;
        while (true) {
            int largest = idx;
            int left = 2 * idx + 1;
            int right = 2 * idx + 2;
            if (left < n && heapData[left] > heapData[largest]) largest = left;
            if (right < n && heapData[right] > heapData[largest]) largest = right;
            if (largest != idx) { std::swap(heapData[idx], heapData[largest]); idx = largest; }
            else break;
        }
    }
}

// 3. Chèn line code vào hàm Insert và HeapifyUp
void HeapTree::insertVal(int val) {
    snapshots.clear(); currentStep = 0; timer = 0;
    
    saveSnapshot("Insert", 0); // Code: heap.push_back(val)
    heapData.push_back(val);
    saveSnapshot("Insert", 1); // Code: index = size - 1
    
    if (heapData.size() > 1) {
        heapifyUp(heapData.size() - 1);
    }
    applyStep(0); 
    isPaused = false;
}

// Thêm hàm này vào cuối file HeapTree.cpp
void HeapTree::searchVal(int val) {
    // Xóa hoạt ảnh cũ, reset bộ đếm
    snapshots.clear(); 
    currentStep = 0; 
    timer = 0;
    
    saveSnapshot("Search", 0); // Highlight code: for (int i = 0; i < heap.size(); ++i)
    
    bool found = false;
    for (int i = 0; i < heapData.size(); ++i) {
        // Highlight code: if (heap[i] == val) và đổi màu Node thứ i sang cam
        saveSnapshotCompare(i, -1, "Search", 1); 
        
        if (heapData[i] == val) {
            // NẾU TÌM THẤY: Tạo snapshot thủ công để set màu Xanh lá cho Node này
            HeapSnapshot snap;
            calculateLayout(snap.nodes);
            snap.nodes[i].color = sf::Color(50, 205, 50); // Màu LimeGreen (Xanh lá)
            snap.operation = "Search";
            snap.activeLine = 2; // Highlight code: return i; // Found
            snapshots.push_back(snap);
            
            found = true;
            break; // Dừng lại khi tìm thấy
        }
    }
    
    if (!found) {
        // Nếu duyệt hết không thấy
        saveSnapshot("Search", 3); // Highlight code: return -1; // Not found
    }
    
    applyStep(0); 
    isPaused = false; // Bắt đầu chạy animation
}

void HeapTree::heapifyUp(int index) {
    int parentIndex = (index - 1) / 2;
    while (index > 0 && heapData[index] > heapData[parentIndex]) {
        saveSnapshotCompare(index, parentIndex, "Insert", 2); // Code: while (heap[i] > heap[parent])
        std::swap(heapData[index], heapData[parentIndex]);
        saveSnapshotSwap(index, parentIndex, "Insert", 3);    // Code: swap(heap[i], heap[parent])
        index = parentIndex;
        parentIndex = (index - 1) / 2;
        saveSnapshot("Insert", 4);                            // Code: i = parent
    }
    saveSnapshot("Insert", -1); // Xong, tắt highlight
}

// 4. Chèn line code vào Delete và HeapifyDown
void HeapTree::deleteMax() {
    if (heapData.empty()) return;
    snapshots.clear(); currentStep = 0; timer = 0;
    
    saveSnapshotCompare(0, heapData.size() - 1, "Delete", 0); // Code: swap(root, last)
    std::swap(heapData[0], heapData[heapData.size() - 1]);
    saveSnapshotSwap(0, heapData.size() - 1, "Delete", 0); 
    
    heapData.pop_back();
    saveSnapshot("Delete", 1); // Code: pop_back()
    
    if (!heapData.empty()) {
        saveSnapshot("Delete", 2); // Code: heapifyDown(0)
        heapifyDown(0);
    }
    applyStep(0); 
    isPaused = false;
}

void HeapTree::heapifyDown(int index) {
    int n = heapData.size();
    while (true) {
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        int leftIdx = (left < n) ? left : -1;
        int rightIdx = (right < n) ? right : -1;
        
        saveSnapshotCompare(largest, leftIdx, "Delete", 3); // Code: find largest child

        if (left < n && heapData[left] > heapData[largest]) largest = left;
        if (right < n) saveSnapshotCompare(largest, rightIdx, "Delete", 3); 
        if (right < n && heapData[right] > heapData[largest]) largest = right;

        saveSnapshot("Delete", 4); // Code: if (largest != index)

        if (largest != index) {
            std::swap(heapData[index], heapData[largest]);
            saveSnapshotSwap(index, largest, "Delete", 5); // Code: swap(heap[index], heap[largest])
            index = largest;
            saveSnapshot("Delete", 6); // Code: index = largest
        } else {
            break;
        }
    }
    saveSnapshot("Delete", -1);
}

void HeapTree::stepForward() {
    if (snapshots.empty()) return;
    
    // Tự động Pause thuật toán và reset lại bộ đếm thời gian
    isPaused = true; 
    timer = 0.0f;    
    
    if (currentStep < snapshots.size() - 1) {
        currentStep++;
        applyStep(currentStep);
    }
}

void HeapTree::stepBackward() {
    if (snapshots.empty()) return;
    
    // Tự động Pause thuật toán và reset lại bộ đếm thời gian
    isPaused = true; 
    timer = 0.0f;    
    
    if (currentStep > 0) {
        currentStep--;
        applyStep(currentStep);
    }
}

void HeapTree::increaseSpeed() { delay = std::max(0.1f, delay - 0.2f); } 
void HeapTree::decreaseSpeed() { delay = std::min(2.0f, delay + 0.2f); } 

void HeapTree::updateAnimation(float deltaTime) {
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

void HeapTree::updatePosition(float deltaTime) {
    for (auto node : visualNodes) {
        node->update(deltaTime);
    }
}

void HeapTree::draw(sf::RenderWindow& window) {
    for (int i = 0; i < visualNodes.size(); ++i) {
        if (!visualNodes[i]->isActive) continue;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < visualNodes.size() && visualNodes[left]->isActive) {
            sf::Vertex line[] = {
                sf::Vertex(visualNodes[i]->currentPos, sf::Color::White),
                sf::Vertex(visualNodes[left]->currentPos, sf::Color::White)
            };
            window.draw(line, 2, sf::Lines);
        }
        if (right < visualNodes.size() && visualNodes[right]->isActive) {
            sf::Vertex line[] = {
                sf::Vertex(visualNodes[i]->currentPos, sf::Color::White),
                sf::Vertex(visualNodes[right]->currentPos, sf::Color::White)
            };
            window.draw(line, 2, sf::Lines);
        }
    }

    for (auto node : visualNodes) {
        node->draw(window);
    }
}

void HeapTree::applyStep(size_t stepIndex) {
    if (snapshots.empty() || stepIndex >= snapshots.size()) return;
    
    for (auto node : visualNodes) node->isActive = false; 

    while (visualNodes.size() < snapshots[stepIndex].nodes.size()) {
        visualNodes.push_back(new HeapNode(0, font)); 
    }

    for (size_t i = 0; i < snapshots[stepIndex].nodes.size(); ++i) {
        const auto& info = snapshots[stepIndex].nodes[i];
        HeapNode* vn = visualNodes[i];
        vn->isActive = true;
        vn->setValue(info.value);
        if (!vn->isDragging) {
            vn->targetPos = info.pos; 
        }
        vn->targetColor = info.color;
    }
}

void HeapTree::handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view) {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseCoords = window.mapPixelToCoords(pixelPos, view);

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        for (auto it = visualNodes.rbegin(); it != visualNodes.rend(); ++it) {
            HeapNode* node = *it;
            if (node->isActive && node->shape.getGlobalBounds().contains(mouseCoords)) {
                draggedNode = node;
                draggedNode->isDragging = true;
                draggedNode->dragOffset = draggedNode->currentPos - mouseCoords; 
                break; 
            }
        }
    } 
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if (draggedNode) {
            draggedNode->isDragging = false;
            draggedNode = nullptr; 
        }
    } 
    else if (event.type == sf::Event::MouseMoved) {
        if (draggedNode && draggedNode->isDragging) {
            draggedNode->currentPos = mouseCoords + draggedNode->dragOffset;
            draggedNode->targetPos = draggedNode->currentPos; 
        }
    }
}

bool HeapTree::isDraggingNode() const {
    return draggedNode != nullptr;
}

// 5. Thêm Getters (để bên UI lấy thông tin) ở cuối file
std::string HeapTree::getCurrentOperation() const {
    if (snapshots.empty() || currentStep >= snapshots.size()) return "";
    return snapshots[currentStep].operation;
}

int HeapTree::getCurrentActiveLine() const {
    if (snapshots.empty() || currentStep >= snapshots.size()) return -1;
    return snapshots[currentStep].activeLine;
}