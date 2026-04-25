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

// Tìm struct StepSnapshot và sửa lại thành:
struct StepSnapshot {
    std::vector<NodeInfo> nodes;
    std::string operation; // Thêm tên thao tác (Insert, Search, Delete)
    int activeLine;        // Thêm dòng code đang chạy
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

    // ... các biến khác giữ nguyên
    std::string currentOpName; // Biến này để cây nhớ nó đang làm gì

    // Cập nhật hàm saveSnapshot để nhận thêm số dòng code
    void saveSnapshot(int line = -1);
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

    // Sửa lại hàm saveSnapshot chỉ nhận 1 tham số dòng code:
    void applyStep(size_t stepIndex);

public:
    AVLTree(sf::Font& font);
    ~AVLTree();

    void initTree(int n);
    void initFromFile(const std::vector<int>& data); // THÊM HÀM NÀY
    void insertVal(int val);
    void deleteVal(int val);
    void searchVal(int val);
    void clear();
    // THÊM 2 HÀM LẤY THÔNG TIN ĐỂ VẼ KHUNG CODE
// ... (các hàm cũ giữ nguyên)
// Đảm bảo 2 hàm này tồn tại để AVLScreen lấy dữ liệu:
    std::string getCurrentOperation() {
        if (snapshots.empty() || currentStep >= (int)snapshots.size()) return "";
        return snapshots[currentStep].operation;
    }
    
    int getCurrentActiveLine() {
        if (snapshots.empty() || currentStep >= (int)snapshots.size()) return -1;
        return snapshots[currentStep].activeLine;
    }
    // ... (Kéo xuống phần private và sửa lại hàm saveSnapshot) ...
    // Playback Controls
    void togglePause() { isPaused = !isPaused; }
    void stepForward();
    void stepBackward();
    void increaseSpeed();
    void decreaseSpeed();
    // THÊM 2 HÀM NÀY:
    void skipToFirstStep(); 
    void skipToLastStep();

    void updatePosition(float deltaTime);
    void updateAnimation(float deltaTime);
    void draw(sf::RenderWindow& window);

    // ... các hàm cũ ...
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view);
    
    // THÊM HÀM NÀY VÀO:
    bool isDraggingNode() const;
};