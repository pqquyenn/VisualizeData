#include "AVLTree.h"
#include <cstdlib>
#include <ctime>

AVLTree::AVLTree(sf::Font& font) : font(font), root(nullptr), currentStep(0), timer(0), delay(0.8f), isPaused(false) {
    std::srand(std::time(0));
}

AVLTree::~AVLTree() { clear(); }

void AVLTree::clear() {
    clearLogical(root);
    root = nullptr;
    for (auto& pair : visualNodes) delete pair.second;
    visualNodes.clear();
    snapshots.clear();
}

void AVLTree::clearLogical(LogicalNode* node) {
    if (!node) return;
    clearLogical(node->left);
    clearLogical(node->right);
    delete node;
}

int AVLTree::height(LogicalNode* n) { return n ? n->height : 0; }
int AVLTree::getBalance(LogicalNode* n) { return n ? height(n->left) - height(n->right) : 0; }
void AVLTree::updateHeight(LogicalNode* n) { if (n) n->height = 1 + std::max(height(n->left), height(n->right)); }

// --- THUẬT TOÁN ĐỊNH TUYẾN TOẠ ĐỘ TRÊN CÂY ---
void AVLTree::calculateLayout(LogicalNode* node, float x, float y, float hGap, std::vector<NodeInfo>& snapshotNodes) {
    if (!node) return;
    NodeInfo info;
    info.value = node->value;
    info.pos = sf::Vector2f(x, y);
    info.color = node->color;
    info.leftVal = node->left ? node->left->value : -1;
    info.rightVal = node->right ? node->right->value : -1;
    snapshotNodes.push_back(info);

    calculateLayout(node->left, x - hGap, y + 80.f, hGap / 2.0f, snapshotNodes);
    calculateLayout(node->right, x + hGap, y + 80.f, hGap / 2.0f, snapshotNodes);
}

void AVLTree::saveSnapshot(int line) {
    StepSnapshot snap;
    
    // 1. PHẦN NÀY LÀ CODE GỐC CỦA BẠN (BẮT BUỘC PHẢI GIỮ LẠI ĐỂ VẼ CÂY)
    // Nó thường là hàm calculateLayout hoặc vòng lặp để đưa node vào snap.nodes
    // Ví dụ:
    calculateLayout(root, 600.f, 150.f, 250.f, snap.nodes); 
    
    // 2. PHẦN MỚI THÊM ĐỂ HIỆN KHUNG CODE VÀNG
    snap.operation = currentOpName; 
    snap.activeLine = line;         
    
    snapshots.push_back(snap);
}


// 2. Thêm hàm initFromFile (đặt dưới hàm initTree)
// Sửa lại hàm initFromFile
void AVLTree::initFromFile(const std::vector<int>& data) {
    clear(); // Xóa cây cũ
    snapshots.clear(); // Làm sạch lịch sử hoạt ảnh
    currentStep = 0;
    
    // Đặt tên thao tác là "Insert" để màn hình hiện khung code màu vàng của Insert
    currentOpName = "Insert"; 

    for (int val : data) {
        resetColors(root);
        saveSnapshot(-1); // Lưu trạng thái trước khi thêm node
        
        // Gọi hàm đệ quy trực tiếp thay vì gọi insertVal() để không bị clear() snapshot
        insertRecursive(root, val); 
        
        resetColors(root);
        saveSnapshot(-1); // Lưu trạng thái sau khi thêm node xong
    }
    
    // Bắt đầu chạy hoạt ảnh từ đầu (node đầu tiên trong file)
    applyStep(0);
    isPaused = false; 
}
// Hàm đẩy cấu hình Step hiện tại ra màn hình
void AVLTree::applyStep(size_t stepIndex) {
    if (stepIndex >= snapshots.size()) return;
    for (auto& pair : visualNodes) pair.second->isActive = false; // Ẩn hết

    for (const auto& info : snapshots[stepIndex].nodes) {
        if (visualNodes.find(info.value) == visualNodes.end()) {
            visualNodes[info.value] = new AVLNode(info.value, font); // Tạo mới nếu chưa có
            visualNodes[info.value]->currentPos = info.pos; // Spawn tại chỗ
        }
        AVLNode* vn = visualNodes[info.value];
        vn->isActive = true;
        vn->targetPos = info.pos;
        vn->targetColor = info.color;
        vn->leftVal = info.leftVal;
        vn->rightVal = info.rightVal;
    }
}

// --- ROTATIONS ---
void AVLTree::rightRotate(LogicalNode*& y) {
    LogicalNode* x = y->left;
    LogicalNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    updateHeight(y);
    updateHeight(x);
    y = x; // Update liên kết cha trực tiếp
}

void AVLTree::leftRotate(LogicalNode*& x) {
    LogicalNode* y = x->right;
    LogicalNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    updateHeight(x);
    updateHeight(y);
    x = y;
}

// --- INSERTION ---
void AVLTree::insertRecursive(LogicalNode*& node, int key) {
    if (node == nullptr) {
        node = new LogicalNode(key);
        node->color = sf::Color(50, 205, 50); // Mới thêm -> Xanh lá
        saveSnapshot(0); // Line 0: if (node == NULL)...
        return;
    }
    
    node->color = sf::Color(255, 165, 0); // Đang duyệt -> Cam
    if (key < node->value) {
        saveSnapshot(1); // Line 1: insert(left)
        insertRecursive(node->left, key);
    } else if (key > node->value) {
        saveSnapshot(2); // Line 2: insert(right)
        insertRecursive(node->right, key);
    } else return;

    updateHeight(node);
    int balance = getBalance(node);
    saveSnapshot(3); // Line 3: update height & balance

    if (balance > 1 || balance < -1) {
        node->color = sf::Color(139, 0, 0); // Mất cân bằng -> Đỏ
        
        if (balance > 1 && key < node->left->value) {
            saveSnapshot(4); // Line 4: LL Case
            rightRotate(node);
        } else if (balance < -1 && key > node->right->value) {
            saveSnapshot(5); // Line 5: RR Case
            leftRotate(node);
        } else if (balance > 1 && key > node->left->value) {
            saveSnapshot(6); // Line 6: LR Case
            leftRotate(node->left); 
            rightRotate(node);
        } else if (balance < -1 && key < node->right->value) {
            saveSnapshot(7); // Line 7: RL Case
            rightRotate(node->right); 
            leftRotate(node);
        }
        saveSnapshot(-1); // Xong xoay
    }
}

// ==================== INSERT ====================
void AVLTree::insertVal(int val) {
    snapshots.clear();
    currentStep = 0;
    currentOpName = "Insert"; // Đánh dấu bắt đầu Insert
    resetColors(root);
    saveSnapshot(-1); 
    insertRecursive(root, val);
    resetColors(root);
    saveSnapshot(-1); 
    applyStep(0);
    isPaused = false;
}
// --- DELETION ---
AVLTree::LogicalNode* AVLTree::minValueNode(LogicalNode* node) {
    LogicalNode* current = node;
    while (current->left != nullptr) current = current->left;
    return current;
}

void AVLTree::deleteRecursive(LogicalNode*& node, int key) {
    if (node == nullptr) {
        saveSnapshot(0);
        return;
    }

    node->color = sf::Color(255, 165, 0);
    if (key < node->value) {
        saveSnapshot(1);
        deleteRecursive(node->left, key);
    } else if (key > node->value) {
        saveSnapshot(2);
        deleteRecursive(node->right, key);
    } else {
        node->color = sf::Color(139, 0, 0); 
        saveSnapshot(3); // Line 3: delete current node

        if ((node->left == nullptr) || (node->right == nullptr)) {
            LogicalNode* temp = node->left ? node->left : node->right;
            if (temp == nullptr) { temp = node; node = nullptr; } 
            else { LogicalNode* oldNode = node; node = temp; temp = oldNode; }
            delete temp;
        } else {
            LogicalNode* temp = minValueNode(node->right);
            node->value = temp->value; 
            deleteRecursive(node->right, temp->value);
        }
    }

    if (node == nullptr) return;

    updateHeight(node);
    int balance = getBalance(node);
    saveSnapshot(4); // Line 4: update height & balance

    if (balance > 1 || balance < -1) {
        node->color = sf::Color(139, 0, 0);
        if (balance > 1 && getBalance(node->left) >= 0) {
            saveSnapshot(5); // LL
            rightRotate(node);
        } else if (balance < -1 && getBalance(node->right) <= 0) {
            saveSnapshot(6); // RR
            leftRotate(node);
        } else if (balance > 1 && getBalance(node->left) < 0) {
            saveSnapshot(7); // LR
            leftRotate(node->left); rightRotate(node);
        } else if (balance < -1 && getBalance(node->right) > 0) {
            saveSnapshot(8); // RL
            rightRotate(node->right); leftRotate(node);
        }
        saveSnapshot(-1);
    }
}

void AVLTree::deleteVal(int val) {
    snapshots.clear();
    currentStep = 0;
    currentOpName = "Delete"; // Đánh dấu bắt đầu Delete
    resetColors(root);
    saveSnapshot(-1);
    deleteRecursive(root, val);
    resetColors(root);
    saveSnapshot(-1);
    applyStep(0);
    isPaused = false;
}

// --- SEARCH ---
void AVLTree::searchRecursive(LogicalNode* node, int key) {
    if (node == nullptr) {
        saveSnapshot(0); // Dòng 0: if (node == NULL) return NOT_FOUND
        return;
    }
    
    node->color = sf::Color(255, 165, 0); // Đang duyệt -> Tô màu Cam
    
    if (node->value == key) {
        node->color = sf::Color(50, 205, 50); // Tìm thấy -> Tô màu Xanh lá
        saveSnapshot(1); // Dòng 1: if (node->key == key) return FOUND
        return;
    }
    
    if (key < node->value) {
        saveSnapshot(2); // Dòng 2: search(node->left, key)
        searchRecursive(node->left, key);
    } else {
        saveSnapshot(3); // Dòng 3: search(node->right, key)
        searchRecursive(node->right, key);
    }
}

void AVLTree::searchVal(int val) {
    snapshots.clear();
    currentStep = 0;
    currentOpName = "Search"; // <--- BÁO HIỆU TƯƠNG TỰ
    resetColors(root);
    saveSnapshot(-1);
    searchRecursive(root, val);
    applyStep(0);
    isPaused = false;
}

void AVLTree::initTree(int n) {
    clear(); // Xóa cây cũ
    snapshots.clear(); // Làm sạch lịch sử hoạt ảnh
    currentStep = 0;
    
    // Đặt tên thao tác là "Insert" để màn hình hiện khung code màu vàng
    currentOpName = "Insert";

    for (int i = 0; i < n; i++) {
        int val = std::rand() % 100 + 1; // Random giá trị
        
        resetColors(root);
        saveSnapshot(-1);
        
        // Gọi hàm đệ quy trực tiếp
        insertRecursive(root, val);
        
        resetColors(root);
        saveSnapshot(-1);
    }
    
    // Bắt đầu chạy hoạt ảnh từ node đầu tiên
    applyStep(0);
    isPaused = false;
}

void AVLTree::resetColors(LogicalNode* node) {
    if (!node) return;
    node->color = sf::Color(70, 130, 180);
    resetColors(node->left);
    resetColors(node->right);
}

// --- PLAYBACK CONTROLS ---
void AVLTree::stepForward() {
    if (snapshots.empty()) return;
    
    // Tự động Pause thuật toán và reset lại bộ đếm thời gian
    isPaused = true; 
    timer = 0.0f;    
    
    if (currentStep < snapshots.size() - 1) {
        currentStep++;
        applyStep(currentStep);
    }
}

void AVLTree::stepBackward() {
    if (snapshots.empty()) return;
    
    // Tự động Pause thuật toán và reset lại bộ đếm thời gian
    isPaused = true; 
    timer = 0.0f;    
    
    if (currentStep > 0) {
        currentStep--;
        applyStep(currentStep);
    }
}

// THÊM 2 HÀM NÀY VÀO DƯỚI stepBackward():
void AVLTree::skipToFirstStep() {
    if (snapshots.empty()) return;
    
    isPaused = true;  // Tự động Pause thuật toán
    timer = 0.0f;     // Reset bộ đếm thời gian
    
    currentStep = 0;  // Nhảy thẳng về bước 0
    applyStep(currentStep);
}

void AVLTree::skipToLastStep() {
    if (snapshots.empty()) return;
    
    isPaused = true;  // Tự động Pause thuật toán
    timer = 0.0f;     // Reset bộ đếm thời gian
    
    currentStep = snapshots.size() - 1; // Nhảy thẳng tới bước cuối cùng
    applyStep(currentStep);
}

void AVLTree::increaseSpeed() { delay = std::max(0.1f, delay - 0.2f); }
void AVLTree::decreaseSpeed() { delay = std::min(2.0f, delay + 0.2f); }

void AVLTree::updateAnimation(float deltaTime) {
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
void AVLTree::updatePosition(float deltaTime) {
    for (auto& pair : visualNodes) pair.second->update(deltaTime);
}

void AVLTree::draw(sf::RenderWindow& window) {
    // Vẽ mũi tên trước để nằm dưới các hình tròn Node
    for (auto& pair : visualNodes) pair.second->drawArrows(window, visualNodes);
    for (auto& pair : visualNodes) pair.second->draw(window);
}

void AVLTree::handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view) {
    // Chỉ cho phép kéo thả nếu không đang chạy hoạt ảnh
    // (hoặc hoạt ảnh đã tạm dừng)
    if (!isPaused && !snapshots.empty() && currentStep < snapshots.size() - 1) return;

    for (auto& pair : visualNodes) {
        pair.second->handleEvent(event, window, view);
    }
}

bool AVLTree::isDraggingNode() const {
    for (const auto& pair : visualNodes) {
        if (pair.second->isActive && pair.second->isDragging) {
            return true;
        }
    }
    return false;
}