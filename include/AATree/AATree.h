#pragma once
#include "AANode.h"
#include <vector>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>

struct AANodeInfo {
    int value;
    sf::Vector2f pos;
    sf::Color color;
    int leftVal;
    int rightVal;
};

struct AAStepSnapshot {
    std::vector<AANodeInfo> nodes;
    std::string operation;
    int activeLine;
};

class AATree {
private:
    struct LogicalNode {
        int value;
        int level;
        LogicalNode* left;
        LogicalNode* right;
        sf::Color color;
        LogicalNode(int v) : value(v), level(1), left(nullptr), right(nullptr), color(sf::Color(70, 130, 180)) {}
    };

    LogicalNode* root;
    sf::Font& font;
    std::map<int, AANode*> visualNodes;
    std::vector<AAStepSnapshot> snapshots;
    size_t currentStep;
    float timer;
    float delay;
    bool isPaused;
    std::string currentOpName;
    // THÊM DÒNG NÀY VÀO TRONG PHẦN PRIVATE:
    float speedMultiplier;
    void saveSnapshot(int line = -1);
    void calculateLayout(LogicalNode* node, float x, float y, float hGap, std::vector<AANodeInfo>& snapshotNodes);
    void applyStep(size_t stepIndex);
    
    // AA Tree Core Operations
    void skew(LogicalNode*& node);
    void split(LogicalNode*& node);
    void decreaseLevel(LogicalNode* node);
    LogicalNode* minValueNode(LogicalNode* node);

    void insertRecursive(LogicalNode*& node, int key);
    void deleteRecursive(LogicalNode*& node, int key);
    void searchRecursive(LogicalNode* node, int key);

    void clearLogical(LogicalNode* node);
    void resetColors(LogicalNode* node);

public:
    AATree(sf::Font& font);
    ~AATree();

    void initTree(int n);
    void initFromFile(const std::vector<int>& data);
    void insertVal(int val);
    void deleteVal(int val);
    void searchVal(int val);
    void clear();

    std::string getCurrentOperation() {
        if (snapshots.empty() || currentStep >= snapshots.size()) return "";
        return snapshots[currentStep].operation;
    }
    int getCurrentActiveLine() {
        if (snapshots.empty() || currentStep >= snapshots.size()) return -1;
        return snapshots[currentStep].activeLine;
    }

    void togglePause() { isPaused = !isPaused; }
    void stepForward();
    void stepBackward();
    void increaseSpeed();
    void decreaseSpeed();
    // THÊM 2 DÒNG NÀY VÀO ĐÂY:
    void skipToFirstStep();
    void skipToLastStep();

    // XoÁ HÀM getDelay() CŨ, THAY BẰNG HÀM NÀY:
    float getSpeedMultiplier() const { return speedMultiplier; }
    void updatePosition(float deltaTime);
    void updateAnimation(float deltaTime);
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view);
    bool isDraggingNode() const;
    void updateVal(int oldVal, int newVal);
};