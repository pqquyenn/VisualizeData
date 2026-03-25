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

void AVLTree::saveSnapshot() {
    StepSnapshot snap;
    // Khởi tạo layout từ giữa màn hình, toạ độ x=600, y=100, khoảng cách gốc = 250
    calculateLayout(root, 600.f, 150.f, 250.f, snap.nodes);
    snapshots.push_back(snap);
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
        saveSnapshot();
        return;
    }
    
    node->color = sf::Color(255, 165, 0); // Đang duyệt -> Cam
    saveSnapshot();

    if (key < node->value) insertRecursive(node->left, key);
    else if (key > node->value) insertRecursive(node->right, key);
    else return;

    updateHeight(node);
    int balance = getBalance(node);

    if (balance > 1 || balance < -1) {
        node->color = sf::Color(139, 0, 0); // Mất cân bằng -> Đỏ
        saveSnapshot();

        if (balance > 1 && key < node->left->value) rightRotate(node);
        else if (balance < -1 && key > node->right->value) leftRotate(node);
        else if (balance > 1 && key > node->left->value) { leftRotate(node->left); rightRotate(node); }
        else if (balance < -1 && key < node->right->value) { rightRotate(node->right); leftRotate(node); }
        
        saveSnapshot(); // Chụp lại sau khi xoay
    }
}

void AVLTree::insertVal(int val) {
    snapshots.clear();
    currentStep = 0;
    resetColors(root);
    saveSnapshot(); // Bắt đầu

    insertRecursive(root, val);

    resetColors(root);
    saveSnapshot(); // Kết thúc
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
    if (node == nullptr) return;

    node->color = sf::Color(255, 165, 0);
    saveSnapshot();

    if (key < node->value) deleteRecursive(node->left, key);
    else if (key > node->value) deleteRecursive(node->right, key);
    else {
        node->color = sf::Color(139, 0, 0); // Tìm thấy node xoá -> Đỏ
        saveSnapshot();

        if ((node->left == nullptr) || (node->right == nullptr)) {
            LogicalNode* temp = node->left ? node->left : node->right;
            if (temp == nullptr) {
                temp = node;
                node = nullptr;
            } else {
                LogicalNode* oldNode = node;
                node = temp; // Nối tắt
                temp = oldNode;
            }
            delete temp;
            saveSnapshot();
        } else {
            LogicalNode* temp = minValueNode(node->right);
            node->value = temp->value; // Copy giá trị
            saveSnapshot();
            deleteRecursive(node->right, temp->value);
        }
    }

    if (node == nullptr) return;

    updateHeight(node);
    int balance = getBalance(node);

    if (balance > 1 || balance < -1) {
        node->color = sf::Color(139, 0, 0); // Unbalanced node
        saveSnapshot();

        if (balance > 1 && getBalance(node->left) >= 0) rightRotate(node);
        else if (balance > 1 && getBalance(node->left) < 0) { leftRotate(node->left); rightRotate(node); }
        else if (balance < -1 && getBalance(node->right) <= 0) leftRotate(node);
        else if (balance < -1 && getBalance(node->right) > 0) { rightRotate(node->right); leftRotate(node); }
        saveSnapshot();
    }
}

void AVLTree::deleteVal(int val) {
    snapshots.clear();
    currentStep = 0;
    resetColors(root);
    saveSnapshot();
    deleteRecursive(root, val);
    resetColors(root);
    saveSnapshot();
    applyStep(0);
    isPaused = false;
}

// --- SEARCH ---
void AVLTree::searchRecursive(LogicalNode* node, int key) {
    if (node == nullptr) return;
    node->color = sf::Color(255, 165, 0);
    saveSnapshot();

    if (node->value == key) {
        node->color = sf::Color(50, 205, 50); // Found
        saveSnapshot();
        return;
    }
    if (key < node->value) searchRecursive(node->left, key);
    else searchRecursive(node->right, key);
}

void AVLTree::searchVal(int val) {
    snapshots.clear();
    currentStep = 0;
    resetColors(root);
    saveSnapshot();
    searchRecursive(root, val);
    applyStep(0);
    isPaused = false;
}

void AVLTree::initTree(int n) {
    clear();
    for (int i = 0; i < n; i++) {
        int val = std::rand() % 100 + 1;
        insertVal(val);
        currentStep = snapshots.size() - 1; // Nhảy thẳng đến cuối
        applyStep(currentStep);
    }
    resetColors(root);
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
    if (currentStep < snapshots.size() - 1) {
        currentStep++;
        applyStep(currentStep);
    }
}

void AVLTree::stepBackward() {
    if (snapshots.empty()) return;
    if (currentStep > 0) {
        currentStep--;
        applyStep(currentStep);
    }
}

void AVLTree::increaseSpeed() { delay = std::max(0.1f, delay - 0.2f); }
void AVLTree::decreaseSpeed() { delay = std::min(2.0f, delay + 0.2f); }

void AVLTree::updateAnimation(float deltaTime) {
    if (snapshots.empty() || isPaused) return;
    timer += deltaTime;
    if (timer >= delay) {
        timer = 0.0f;
        if (currentStep < snapshots.size() - 1) stepForward();
        else isPaused = true;
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