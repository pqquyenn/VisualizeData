#include "AATree.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

AATree::AATree(sf::Font& font) : font(font), root(nullptr), currentStep(0), timer(0), delay(0.8f), isPaused(false) {
    // THÊM DÒNG NÀY ĐỂ KHỞI TẠO TỐC ĐỘ MẶC ĐỊNH LÀ 1.0x
    speedMultiplier = 1.0f;
    std::srand(std::time(0));
}

AATree::~AATree() { clear(); }

void AATree::clear() {
    clearLogical(root);
    root = nullptr;
    for (auto& pair : visualNodes) delete pair.second;
    visualNodes.clear();
    snapshots.clear();
}

void AATree::clearLogical(LogicalNode* node) {
    if (!node) return;
    clearLogical(node->left);
    clearLogical(node->right);
    delete node;
}

void AATree::calculateLayout(LogicalNode* node, float x, float y, float hGap, std::vector<AANodeInfo>& snapshotNodes) {
    if (!node) return;
    AANodeInfo info;
    info.value = node->value;
    info.pos = sf::Vector2f(x, y);
    info.color = node->color;
    info.leftVal = node->left ? node->left->value : -1;
    info.rightVal = node->right ? node->right->value : -1;
    snapshotNodes.push_back(info);

    calculateLayout(node->left, x - hGap, y + 80.f, hGap / 2.0f, snapshotNodes);
    calculateLayout(node->right, x + hGap, y + 80.f, hGap / 2.0f, snapshotNodes);
}

void AATree::saveSnapshot(int line) {
    AAStepSnapshot snap;
    calculateLayout(root, 640.f, 150.f, 250.f, snap.nodes); 
    snap.operation = currentOpName; 
    snap.activeLine = line;        
    snapshots.push_back(snap);
}

void AATree::applyStep(size_t stepIndex) {
    if (stepIndex >= snapshots.size()) return;
    for (auto& pair : visualNodes) pair.second->isActive = false; 

    for (const auto& info : snapshots[stepIndex].nodes) {
        if (visualNodes.find(info.value) == visualNodes.end()) {
            visualNodes[info.value] = new AANode(info.value, font); 
            visualNodes[info.value]->currentPos = info.pos; 
        }
        AANode* vn = visualNodes[info.value];
        vn->isActive = true;
        vn->targetPos = info.pos;
        vn->targetColor = info.color;
        vn->leftVal = info.leftVal;
        vn->rightVal = info.rightVal;
    }
}

// --- AA Tree Logic ---
void AATree::skew(LogicalNode*& node) {
    if (!node || !node->left) return;
    if (node->left->level == node->level) {
        LogicalNode* l = node->left;
        node->left = l->right;
        l->right = node;
        node = l;
    }
}

void AATree::split(LogicalNode*& node) {
    if (!node || !node->right || !node->right->right) return;
    if (node->level == node->right->right->level) {
        LogicalNode* r = node->right;
        node->right = r->left;
        r->left = node;
        r->level++;
        node = r;
    }
}

void AATree::decreaseLevel(LogicalNode* node) {
    int expected = 1;
    if (node->left && node->right) expected = std::min(node->left->level, node->right->level) + 1;
    else if (node->left) expected = node->left->level + 1;
    
    if (expected < node->level) {
        node->level = expected;
        if (node->right && expected < node->right->level) {
            node->right->level = expected;
        }
    }
}

AATree::LogicalNode* AATree::minValueNode(LogicalNode* node) {
    LogicalNode* current = node;
    while (current->left) current = current->left;
    return current;
}

// --- CORE OPERATIONS ---
void AATree::insertRecursive(LogicalNode*& node, int key) {
    if (!node) {
        node = new LogicalNode(key);
        node->color = sf::Color(50, 205, 50); 
        saveSnapshot(0); 
        return;
    }
    
    node->color = sf::Color(255, 165, 0); 
    if (key < node->value) {
        saveSnapshot(1); 
        insertRecursive(node->left, key);
    } else if (key > node->value) {
        saveSnapshot(2); 
        insertRecursive(node->right, key);
    } else return;

    node->color = sf::Color(139, 0, 0); 
    skew(node);
    saveSnapshot(3); 
    
    split(node);
    saveSnapshot(4); 
}

void AATree::deleteRecursive(LogicalNode*& node, int key) {
    if (!node) { saveSnapshot(0); return; }

    node->color = sf::Color(255, 165, 0);
    if (key < node->value) {
        saveSnapshot(1);
        deleteRecursive(node->left, key);
    } else if (key > node->value) {
        saveSnapshot(2);
        deleteRecursive(node->right, key);
    } else {
        node->color = sf::Color(139, 0, 0);
        saveSnapshot(3);
        if (!node->left && !node->right) {
            delete node; node = nullptr;
        } else if (!node->left) {
            LogicalNode* temp = node; node = node->right; delete temp;
        } else if (!node->right) {
            LogicalNode* temp = node; node = node->left; delete temp;
        } else {
            LogicalNode* succ = minValueNode(node->right);
            node->value = succ->value;
            deleteRecursive(node->right, succ->value);
        }
    }

    if (!node) return;

    decreaseLevel(node);
    saveSnapshot(4);

    skew(node);
    if (node->right) skew(node->right);
    if (node->right && node->right->right) skew(node->right->right);
    saveSnapshot(5);

    split(node);
    if (node->right) split(node->right);
    saveSnapshot(6);
}

void AATree::searchRecursive(LogicalNode* node, int key) {
    if (!node) { saveSnapshot(0); return; }
    node->color = sf::Color(255, 165, 0); 
    
    if (node->value == key) {
        node->color = sf::Color(50, 205, 50); 
        saveSnapshot(1); 
        return;
    }
    
    if (key < node->value) {
        saveSnapshot(2); 
        searchRecursive(node->left, key);
    } else {
        saveSnapshot(3); 
        searchRecursive(node->right, key);
    }
}

// --- PUBLIC APIS ---
void AATree::insertVal(int val) {
    snapshots.clear(); currentStep = 0; currentOpName = "Insert";
    resetColors(root);
    saveSnapshot(-1); 
    insertRecursive(root, val);
    resetColors(root);
    saveSnapshot(-1); 
    applyStep(0);
    isPaused = false;
}

void AATree::deleteVal(int val) {
    snapshots.clear(); currentStep = 0; currentOpName = "Delete";
    resetColors(root);
    saveSnapshot(-1);
    deleteRecursive(root, val);
    resetColors(root);
    saveSnapshot(-1);
    applyStep(0);
    isPaused = false;
}

void AATree::searchVal(int val) {
    snapshots.clear(); currentStep = 0; currentOpName = "Search";
    resetColors(root);
    saveSnapshot(-1);
    searchRecursive(root, val);
    applyStep(0);
    isPaused = false;
}

void AATree::initTree(int n) {
    clear(); snapshots.clear(); currentStep = 0; currentOpName = "Insert";
    for (int i = 0; i < n; i++) {
        int val = std::rand() % 100 + 1;
        resetColors(root);
        saveSnapshot(-1);
        insertRecursive(root, val);
        resetColors(root);
        saveSnapshot(-1);
    }
    applyStep(0);
    isPaused = false;
}

void AATree::initFromFile(const std::vector<int>& data) {
    clear(); snapshots.clear(); currentStep = 0; currentOpName = "Insert";
    for (int val : data) {
        resetColors(root);
        saveSnapshot(-1);
        insertRecursive(root, val);
        resetColors(root);
        saveSnapshot(-1);
    }
    applyStep(0);
    isPaused = false;
}

void AATree::resetColors(LogicalNode* node) {
    if (!node) return;
    node->color = sf::Color(70, 130, 180);
    resetColors(node->left);
    resetColors(node->right);
}

// --- ANIMATION CONTROLS ---
void AATree::stepForward() {
    if (snapshots.empty()) return;
    
    // Tự động Pause thuật toán và reset lại bộ đếm thời gian
    isPaused = true; 
    timer = 0.0f;    
    
    if (currentStep < snapshots.size() - 1) {
        currentStep++;
        applyStep(currentStep);
    }
}

void AATree::stepBackward() {
    if (snapshots.empty()) return;
    
    // Tự động Pause thuật toán và reset lại bộ đếm thời gian
    isPaused = true; 
    timer = 0.0f;    
    
    if (currentStep > 0) {
        currentStep--;
        applyStep(currentStep);
    }
}

void AATree::skipToFirstStep() {
    if (snapshots.empty()) return;
    
    isPaused = true;  // Tự động Pause thuật toán
    timer = 0.0f;     // Reset bộ đếm thời gian
    
    currentStep = 0;  // Nhảy về bước đầu tiên
    applyStep(currentStep);
}

void AATree::skipToLastStep() {
    if (snapshots.empty()) return;
    
    isPaused = true;  // Tự động Pause thuật toán
    timer = 0.0f;     // Reset bộ đếm thời gian
    
    currentStep = snapshots.size() - 1; // Nhảy tới bước cuối cùng
    applyStep(currentStep);
}
void AATree::increaseSpeed() { 
    speedMultiplier += 0.25f;
    if (speedMultiplier > 3.0f) speedMultiplier = 3.0f; // Chặn tối đa ở mức 3.0x
    
    delay = 0.8f / speedMultiplier; // Tính lại delay dựa trên hệ số tốc độ
}

void AATree::decreaseSpeed() { 
    speedMultiplier -= 0.25f;
    if (speedMultiplier < 0.25f) speedMultiplier = 0.25f; // Chặn tối thiểu ở mức 0.25x
    
    delay = 0.8f / speedMultiplier; // Tính lại delay dựa trên hệ số tốc độ
}

void AATree::updateAnimation(float deltaTime) {
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
void AATree::updatePosition(float deltaTime) {
    for (auto& pair : visualNodes) pair.second->update(deltaTime);
}

void AATree::draw(sf::RenderWindow& window) {
    for (auto& pair : visualNodes) pair.second->drawArrows(window, visualNodes);
    for (auto& pair : visualNodes) pair.second->draw(window);
}

void AATree::handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view) {
    if (!isPaused && !snapshots.empty() && currentStep < snapshots.size() - 1) return;
    for (auto& pair : visualNodes) pair.second->handleEvent(event, window, view);
}

void AATree::updateVal(int oldVal, int newVal) {
    snapshots.clear();
    currentStep = 0;

    // Bước 1: Kiểm tra xem giá trị cũ (oldVal) có tồn tại trong cây không
    bool exists = false;
    LogicalNode* temp = root;
    while (temp) {
        if (temp->value == oldVal) { exists = true; break; }
        if (oldVal < temp->value) temp = temp->left;
        else temp = temp->right;
    }

    // Nếu giá trị cũ không tồn tại, chỉ chạy animation Search báo không tìm thấy
    if (!exists) {
        currentOpName = "Search";
        resetColors(root);
        saveSnapshot(-1);
        searchRecursive(root, oldVal);
        applyStep(0);
        isPaused = false;
        return;
    }

    // Nếu tồn tại, thực hiện liên hoàn Xoá node cũ -> Thêm node mới
    currentOpName = "Delete";
    resetColors(root);
    saveSnapshot(-1);
    deleteRecursive(root, oldVal);

    currentOpName = "Insert";
    resetColors(root);
    saveSnapshot(-1);
    insertRecursive(root, newVal);

    resetColors(root);
    saveSnapshot(-1);

    applyStep(0);
    isPaused = false;
}

bool AATree::isDraggingNode() const {
    for (const auto& pair : visualNodes) {
        if (pair.second->isActive && pair.second->isDragging) return true;
    }
    return false;
}