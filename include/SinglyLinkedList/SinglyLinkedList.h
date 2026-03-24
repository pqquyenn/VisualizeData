#pragma once
#include "ListNode.h"
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

struct SLLNodeInfo {
    int id;
    int value;
    sf::Vector2f pos;
    sf::Color color;
    int nextId;
};

struct SLLStepSnapshot {
    std::vector<SLLNodeInfo> nodes;
};

class SinglyLinkedList {
private:
    struct LogicalNode {
        int id;
        int value;
        sf::Color color;
        LogicalNode* next;
        LogicalNode(int id, int v) : id(id), value(v), color(sf::Color(70, 130, 180)), next(nullptr) {}
    };

    LogicalNode* head;
    int idCounter; // Định danh độc nhất cho mỗi node, chống lỗi khi nhập trùng value
    sf::Font& font;
    std::map<int, ListNode*> visualNodes;

    std::vector<SLLStepSnapshot> snapshots;
    size_t currentStep;
    float timer;
    float delay;
    bool isPaused;

    float startX = 100.f;
    float startY = 350.f;
    float nodeSpacing = 110.f;

    void saveSnapshot();
    void applyStep(size_t stepIndex);
    void resetColors();
    void clearLogical();

public:
    SinglyLinkedList(sf::Font& font);
    ~SinglyLinkedList();

    void initList(int n);
    void insertNode(int val);
    void startSearch(int val);
    void startDelete(int val);
    void clear();

    // Playback control
    void togglePause() { isPaused = !isPaused; }
    void stepForward();
    void stepBackward();
    void increaseSpeed();
    void decreaseSpeed();

    void updatePosition(float deltaTime);
    void updateAnimation(float deltaTime); 
    void draw(sf::RenderWindow& window);
};