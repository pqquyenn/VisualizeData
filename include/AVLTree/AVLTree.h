#pragma once
#include "AVLNode.h"
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

// Lưu trữ trạng thái hiển thị của một node tại một thời điểm
struct NodeInfo {
    int value;
    sf::Vector2f pos;
    sf::Color color;
    int leftVal;
    int rightVal;
};

// Lưu trữ toàn bộ khung hình của 1 bước hoạt ảnh
struct StepSnapshot {
    std::vector<NodeInfo> nodes;
};

class AVLTree {
private:
    struct LogicalNode {
        int value;
        int height;
        LogicalNode* left;
        LogicalNode* right;
        sf::Color color;
        LogicalNode(int v) : value(v), height(1), left(nullptr), right(nullptr), color(sf::Color(70, 130, 180)) {}
    };

    LogicalNode* root;
    sf::Font& font;
    std::map<int, AVLNode*> visualNodes; // Quản lý object SFML

    std::vector<StepSnapshot> snapshots;
    size_t currentStep;
    float timer;
    float delay;
    bool isPaused;

    // Các hàm Helper Tree Logic (Dùng reference pointer để cập nhật trực tiếp)
    int height(LogicalNode* n);
    int getBalance(LogicalNode* n);
    void updateHeight(LogicalNode* n);
    
    void rightRotate(LogicalNode*& y);
    void leftRotate(LogicalNode*& x);
    
    void insertRecursive(LogicalNode*& node, int key);
    void deleteRecursive(LogicalNode*& node, int key);
    LogicalNode* minValueNode(LogicalNode* node);
    void searchRecursive(LogicalNode* node, int key);

    void clearLogical(LogicalNode* node);
    void resetColors(LogicalNode* node);
    
    // Core Animation Builder
    void calculateLayout(LogicalNode* node, float x, float y, float hGap, std::vector<NodeInfo>& snapshotNodes);
    void saveSnapshot();
    void applyStep(size_t stepIndex);

public:
    AVLTree(sf::Font& font);
    ~AVLTree();

    void initTree(int n);
    void insertVal(int val);
    void deleteVal(int val);
    void searchVal(int val);
    void clear();

    // Playback Controls
    void togglePause() { isPaused = !isPaused; }
    void stepForward();
    void stepBackward();
    void increaseSpeed();
    void decreaseSpeed();

    void updatePosition(float deltaTime);
    void updateAnimation(float deltaTime);
    void draw(sf::RenderWindow& window);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view);
};