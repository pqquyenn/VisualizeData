#include "Heap/HeapTree.h"
#include <cmath>
#include <algorithm>
#include <ctime>

// --- FILE: src/Heap/HeapTree.cpp ---

HeapTree::HeapTree(sf::Font& f) : font(f), currentStep(0), timer(0), delay(0.8f), isPaused(true) {
    std::srand(std::time(0));
}

HeapTree::~HeapTree() { clear(); }

void HeapTree::clear() {
    heapData.clear();
    for (auto node : visualNodes) delete node;
    visualNodes.clear();
    snapshots.clear();
    currentStep = 0;
}

void HeapTree::calculateLayout(std::vector<HeapNodeInfo>& snapshotNodes) {
    snapshotNodes.clear();
    if (heapData.empty()) return;

    // Căn giữa layout: Gốc tại (600, 150), gap Y=80, gap X ban đầu=250 (giảm dần theo độ sâu)
    float startX = 600.0f; 
    float startY = 150.0f;
    float yGap = 80.0f;

    for (int i = 0; i < heapData.size(); ++i) {
        int level = std::log2(i + 1);
        int nodesInLevel = std::pow(2, level);
        int indexInLevel = i - (nodesInLevel - 1);
        
        // Khoảng cách X giữa các node ở cùng tầng giảm dần
        float xGap = 250.0f / std::pow(2, level); 
        float x = startX - (nodesInLevel - 1) * xGap / 2.0f + indexInLevel * xGap;
        float y = startY + level * yGap;

        snapshotNodes.push_back({heapData[i], sf::Vector2f(x, y), sf::Color(70, 130, 180)}); // Màu mặc định
    }
}

void HeapTree::saveSnapshot() {
    HeapSnapshot snap;
    calculateLayout(snap.nodes);
    snapshots.push_back(snap);
}

// Lưu snapshot so sánh: 2 node vàng sáng lên
void HeapTree::saveSnapshotCompare(int idx1, int idx2) {
    HeapSnapshot snap;
    calculateLayout(snap.nodes);
    if (idx1 < snap.nodes.size()) snap.nodes[idx1].color = sf::Color(255, 165, 0); // Vàng/Cam
    if (idx2 < snap.nodes.size()) snap.nodes[idx2].color = sf::Color(255, 165, 0);
    snapshots.push_back(snap);
}

// Lưu snapshot sau khi swap dữ liệu, visual nodes bắt đầu di chuyển
void HeapTree::saveSnapshotSwap(int idx1, int idx2) {
    HeapSnapshot snap;
    calculateLayout(snap.nodes); // Tính toán lại vị trí đích của cây *mới* sau swap
    // Giữ màu vàng sau khi swap để thấy node di chuyển
    if (idx1 < snap.nodes.size()) snap.nodes[idx1].color = sf::Color(255, 165, 0);
    if (idx2 < snap.nodes.size()) snap.nodes[idx2].color = sf::Color(255, 165, 0);
    snapshots.push_back(snap);
}

void HeapTree::applyStep(size_t stepIndex) {
    if (snapshots.empty() || stepIndex >= snapshots.size()) return;
    
    // Ẩn hết node cũ
    for (auto node : visualNodes) node->isActive = false; 

    // Cập nhật các visual nodes từ snapshot
    while (visualNodes.size() < snapshots[stepIndex].nodes.size()) {
        visualNodes.push_back(new HeapNode(0, font)); // Tạo mới nếu thiếu
    }

    for (size_t i = 0; i < snapshots[stepIndex].nodes.size(); ++i) {
        const auto& info = snapshots[stepIndex].nodes[i];
        HeapNode* vn = visualNodes[i];
        vn->isActive = true;
        vn->value = info.value;
        vn->text.setString(std::to_string(vn->value));
        
        vn->targetPos = info.pos; // Đặt đích cho hoạt ảnh
        vn->targetColor = info.color;
    }
}

// --- SỬA: Chức năng Init theo số lượng N linh hoạt ---
void HeapTree::initTree(int n) {
    clear(); // Xóa cây cũ
    // Tạo mảng n node ngẫu nhiên
    for (int i = 0; i < n; i++) {
        heapData.push_back(std::rand() % 100 + 1);
    }
    // Xây dựng heap tực thời O(N) (Không tạo hoạt ảnh cho bước Init)
    buildHeap();
    
    // Lưu một snapshot cuối cùng để hiển thị ngay lập tức
    saveSnapshot(); 
    isPaused = true; 
    applyStep(0);   
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

void HeapTree::insertVal(int val) {
    snapshots.clear(); currentStep = 0; timer = 0;
    
    // --- BƯỚC 1: Thêm vào cuối mảng và lưu snapshot ban đầu ---
    heapData.push_back(val);
    saveSnapshot(); // Snapshot lúc vừa thêm

    // Bắt đầu heapify up từ node cuối
    if (heapData.size() > 1) {
        heapifyUp(heapData.size() - 1);
    }

    applyStep(0); 
    isPaused = false;
}

void HeapTree::heapifyUp(int index) {
    int parentIndex = (index - 1) / 2;
    while (index > 0 && heapData[index] > heapData[parentIndex]) {
        // --- BƯỚC 2: Snapshot so sánh: 2 Node so sánh màu vàng ---
        saveSnapshotCompare(index, parentIndex); 

        // --- BƯỚC 3: Hoán đổi dữ liệu ---
        std::swap(heapData[index], heapData[parentIndex]);

        // --- BƯỚC 4: Snapshot hoán đổi: Visual nodes bắt đầu di chuyển đến vị trí đích mới ---
        saveSnapshotSwap(index, parentIndex); // Giữ màu vàng

        // Cập nhật chỉ số và tiếp tục
        index = parentIndex;
        parentIndex = (index - 1) / 2;
    }
    // --- Snapshot cuối cùng: Trạng thái ổn định (màu xanh mặc định) ---
    saveSnapshot();
}

void HeapTree::deleteMax() {
    if (heapData.empty()) return;
    snapshots.clear(); currentStep = 0; timer = 0;

    // --- BƯỚC 1: Snapshot ban đầu ---
    saveSnapshot(); 

    // --- BƯỚC 2: Swap gốc và node cuối (So sánh & Swap animation) ---
    saveSnapshotCompare(0, heapData.size() - 1); // Hiện vàng
    std::swap(heapData[0], heapData[heapData.size() - 1]);
    saveSnapshotSwap(0, heapData.size() - 1); // Visual nodes di chuyển sau swap

    // --- BƯỚC 3: Xóa node cuối ---
    heapData.pop_back();
    saveSnapshot(); // Snapshot sau khi xóa node

    // Bắt đầu heapify down từ gốc mới
    if (!heapData.empty()) {
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

        // --- BƯỚC 4: Snapshot so sánh: Cha với con trái/phải ---
        int leftIdx = (left < n) ? left : -1;
        int rightIdx = (right < n) ? right : -1;
        saveSnapshotCompare(largest, leftIdx); // So sánh cha-con trái

        if (left < n && heapData[left] > heapData[largest]) largest = left;
        
        if (right < n) saveSnapshotCompare(largest, rightIdx); // So sánh (hoặc giữ) cha-con phải
        if (right < n && heapData[right] > heapData[largest]) largest = right;

        if (largest != index) {
            // --- BƯỚC 5: Hoán đổi và Snapshot hoán đổi ---
            std::swap(heapData[index], heapData[largest]);
            saveSnapshotSwap(index, largest); // Visual nodes di chuyển
            index = largest;
        } else {
            break;
        }
    }
    // --- Snapshot cuối cùng: Trạng thái ổn định xanh ---
    saveSnapshot();
}

void HeapTree::stepForward() {
    if (snapshots.empty()) return;
    if (currentStep < snapshots.size() - 1) {
        currentStep++;
        applyStep(currentStep);
    }
}

void HeapTree::stepBackward() {
    if (snapshots.empty()) return;
    if (currentStep > 0) {
        currentStep--;
        applyStep(currentStep);
    }
}

// --- TÍNH NĂNG SPEED (Sử dụng logic từ AVLTree của bạn) ---
void HeapTree::increaseSpeed() { delay = std::max(0.1f, delay - 0.2f); } // Tốc độ nhanh hơn -> delay giảm
void HeapTree::decreaseSpeed() { delay = std::min(2.0f, delay + 0.2f); } // Tốc độ chậm hơn -> delay tăng

void HeapTree::updateAnimation(float deltaTime) {
    if (snapshots.empty() || isPaused) return;
    timer += deltaTime;
    if (timer >= delay) {
        timer = 0.0f;
        if (currentStep < snapshots.size() - 1) stepForward();
        else isPaused = true;
    }
}

void HeapTree::updatePosition(float deltaTime) {
    for (auto node : visualNodes) {
        node->update(deltaTime);
    }
}

void HeapTree::draw(sf::RenderWindow& window) {
    // Vẽ đường nối (Cạnh của cây)
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

    // Vẽ Node
    for (auto node : visualNodes) {
        node->draw(window);
    }
}

void HeapTree::handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view) {
    // Chỉ cho phép kéo thả nếu cây đang tạm dừng
    if (!isPaused && !snapshots.empty() && currentStep < snapshots.size() - 1) return;

    for (auto node : visualNodes) {
        node->handleEvent(event, window, view);
    }
}