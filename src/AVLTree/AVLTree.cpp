#include "AVLTree.h"
#include <algorithm> // cho std::max

AVLTree::AVLTree(sf::Font& font) : font(font), root(nullptr), currentStep(-1) {}

AVLTree::~AVLTree() { clear(root); }

void AVLTree::clear(AVLNode* node) {
    if (node) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

// ================= CÁC HÀM TIỆN ÍCH AVL =================
int AVLTree::height(AVLNode* node) {
    return node ? node->height : 0;
}

int AVLTree::getBalance(AVLNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

// ================= THUẬT TOÁN XOAY CÂY =================
AVLNode* AVLTree::rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;

    return x;
}

AVLNode* AVLTree::leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;

    return y;
}

// ================= TÍNH TOẠ ĐỘ NODE (LAYOUT) =================
// Hàm này cực kỳ quan trọng để chia đều không gian cho nhánh trái/phải
void AVLTree::calculateLayout(AVLNode* node, float x, float y, float horizontalSpacing) {
    if (!node) return;
    
    node->targetPos = sf::Vector2f(x, y);
    
    // Nếu node chưa có toạ độ hiện tại (vừa được tạo), cho nó xuất phát từ trên trời rớt xuống
    if (node->currentPos.x == 0 && node->currentPos.y == 0) {
        node->currentPos = sf::Vector2f(x, y - 200.0f);
    }

    // Đệ quy tính cho 2 con, khoảng cách X giảm dần theo độ sâu
    calculateLayout(node->left, x - horizontalSpacing, y + 80.0f, horizontalSpacing * 0.5f);
    calculateLayout(node->right, x + horizontalSpacing, y + 80.0f, horizontalSpacing * 0.5f);
}

// ================= LƯU LỊCH SỬ (SNAPSHOT) =================
// Hàm này tạm thời đang rỗng, để tích hợp History hoàn chỉnh ta sẽ cần viết một hàm đệ quy 
// duyệt qua toàn bộ node hiện tại và copy thông tin đẩy vào mảng `history`.
// Tạm thời gọi hàm layout mỗi khi lưu trạng thái.
void AVLTree::saveState() {
    // 1. Tính toán lại toàn bộ vị trí dựa trên cấu trúc cây hiện tại
    // Điểm neo root ở toạ độ (600, 150), khoảng cách ban đầu 300
    calculateLayout(root, 600.0f, 150.0f, 300.0f);
    
    // Tương lai: Thêm logic copy state vào mảng history ở đây để Playback hoạt động
}

// ================= CHỨC NĂNG CHÍNH MÀ UI GỌI =================
AVLNode* AVLTree::insertNode(AVLNode* node, int val) {
    if (node == nullptr) {
        AVLNode* newNode = new AVLNode(val, font);
        newNode->currentColor = sf::Color(255, 165, 0); // Màu cam khi mới chèn
        return newNode;
    }

    if (val < node->val)
        node->left = insertNode(node->left, val);
    else if (val > node->val)
        node->right = insertNode(node->right, val);
    else return node; // Không cho phép trùng

    // Cập nhật chiều cao
    node->height = 1 + std::max(height(node->left), height(node->right));

    // Kiểm tra cân bằng
    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && val < node->left->val)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && val > node->right->val)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && val > node->left->val) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && val < node->right->val) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void AVLTree::insert(int val) {
    root = insertNode(root, val);
    saveState(); // Cập nhật lại layout và lưu lịch sử sau khi Insert + Xoay (nếu có)
}

// Hàm update đệ quy cập nhật animation cho từng node
void updateNodeAnim(AVLNode* node, float dt) {
    if (!node) return;
    node->updateAnimation(dt);
    updateNodeAnim(node->left, dt);
    updateNodeAnim(node->right, dt);
}

void AVLTree::update(float deltaTime) {
    updateNodeAnim(root, deltaTime);
}

void drawNodeInfo(AVLNode* node, sf::RenderWindow& window) {
    if (!node) return;
    node->draw(window); // Draw con trái/phải trước, sau đó vẽ chính nó
    drawNodeInfo(node->left, window);
    drawNodeInfo(node->right, window);
}

void AVLTree::draw(sf::RenderWindow& window) {
    drawNodeInfo(root, window);
}

// Tạm thời các hàm này để trống để bạn hoàn thiện sau
void AVLTree::remove(int val) {}
void AVLTree::search(int val) {}
void AVLTree::initRandom(int n) {}
void AVLTree::stepForward() {}
void AVLTree::stepBackward() {}