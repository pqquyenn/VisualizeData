#pragma once
#include "ListNode.h"
#include <vector>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>

struct SLLNodeInfo {
    int id;
    int value;
    sf::Vector2f pos;
    sf::Color color;
    int nextId;
};

// Lưu giữ khoảnh khắc trạng thái của Nodes và Line code đang chạy
struct SLLStepSnapshot {
    std::vector<SLLNodeInfo> nodes;
    std::vector<std::string> codeLines; 
    int highlightLine;                  
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
    int idCounter; 
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

    // Các thành phần UI vẽ Box Code
    sf::RectangleShape codeBox;
    sf::Text codeText;
    std::vector<std::string> currentCode;
    int currentHighlight;

    void saveSnapshot(const std::vector<std::string>& code = {}, int line = -1);
    void applyStep(size_t stepIndex);
    void resetColors();
    void clearLogical();

public:
    SinglyLinkedList(sf::Font& font);
    ~SinglyLinkedList();

    void initList(int n);
    void initFromFile(const std::string& filename); // Thêm đọc file
    void insertNode(int val);
    void startSearch(int val);
    void startDelete(int val);
    void clear();

    void togglePause() { isPaused = !isPaused; }
    void stepForward();
    void stepBackward();
    void increaseSpeed();
    void decreaseSpeed();

    void updatePosition(float deltaTime);
    void updateAnimation(float deltaTime); 
    void draw(sf::RenderWindow& window);
    void drawCode(sf::RenderWindow& window); // Hàm tĩnh để vẽ UI code
};