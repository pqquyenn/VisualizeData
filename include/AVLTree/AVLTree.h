#pragma once
#include "AVLNode.h"
#include <vector>

// Struct dùng để "chụp ảnh" lại 1 node tại 1 thời điểm
struct NodeSnapshot {
    int val;
    sf::Vector2f pos;
    sf::Color color;
    int leftChildVal;  // Dùng -1 nếu không có
    int rightChildVal; // Dùng -1 nếu không có
};

// Lưu toàn bộ cây tại 1 bước (1 frame trong lịch sử)
struct TreeSnapshot {
    std::vector<NodeSnapshot> nodes;
};

class AVLTree {
private:
    AVLNode* root;
    sf::Font& font;
    
    // Hệ thống lịch sử cho Playback
    std::vector<TreeSnapshot> history;
    int currentStep;

    // Các hàm Logic AVL nội bộ (Đệ quy)
    int height(AVLNode* node);
    int getBalance(AVLNode* node);
    AVLNode* rightRotate(AVLNode* y);
    AVLNode* leftRotate(AVLNode* x);
    
    AVLNode* insertNode(AVLNode* node, int val);
    AVLNode* deleteNode(AVLNode* root, int val);
    
    // Hàm tính toán toạ độ (Layout) để cây không bị đè nhau
    void calculateLayout(AVLNode* node, float x, float y, float horizontalSpacing);

    // Hàm dọn dẹp bộ nhớ
    void clear(AVLNode* node);

public:
    AVLTree(sf::Font& font);
    ~AVLTree();

    // Các hàm gọi từ UI
    void insert(int val);
    void remove(int val);
    void search(int val);
    void initRandom(int n);

    // Xử lý Playback
    void saveState(); 
    void stepForward();
    void stepBackward();
    void resetToCurrentState(); // Áp dụng Snapshot hiện tại lên UI

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
};