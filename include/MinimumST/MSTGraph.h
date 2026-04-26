#pragma once
#include "MSTNode.h"
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

struct Edge {
    int u, v, weight;
    sf::Color color = sf::Color(200, 200, 200); // Màu xám nhạt mặc định
};

// Cấu trúc hỗ trợ tìm chu trình cho Kruskal
struct DisjointSet {
    std::map<int, int> parent;
    void makeSet(int v) { parent[v] = v; }
    int find(int v) {
        if (parent[v] == v) return v;
        return parent[v] = find(parent[v]);
    }
    void unionSet(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        if (rootU != rootV) parent[rootU] = rootV;
    }
};

struct MSTState {
    std::vector<sf::Color> edgeColors;
    std::vector<std::string> codeLines;
    int highlightLine = -1;
};

class MSTGraph {
private:
    sf::Font& font;
    std::map<int, MSTNode*> nodes;
    std::vector<Edge> edges;

    // --- CẬP NHẬT: Thay đổi kiểu vector animationSteps ---
    std::vector<MSTState> animationSteps; 
    int currentStep = 0;
    float timer = 0.0f;
    float stepDuration = 1.0f; 
    bool isPaused = false;
    bool isAnimating = false;

    float speedMultiplier;

    // --- THÊM MỚI: Các biến cho giao diện hiển thị Code ---
    sf::RectangleShape codeBox;
    sf::Text codeText;
    std::vector<std::string> currentCode;
    int currentHighlight = -1;

    void addNode(int id);
    void arrangeCircularLayout();

public:
    MSTGraph(sf::Font& font);
    ~MSTGraph();

    void clear();
    bool buildFromEdgeList(const std::string& input);
    bool buildFromAdjMatrix(const std::string& input, int numVertices);
    bool buildFromAdjList(const std::string& input);
    void generateRandomGraph(int v, int e);

    void startKruskal();
    void togglePause();
    void stepForward();
    void stepBackward();
    void increaseSpeed();
    void decreaseSpeed();

    float getSpeedMultiplier() const { return speedMultiplier; }

    void skipToFirstStep();
    void skipToLastStep();

    void update(float dt);
    void draw(sf::RenderWindow& window);
    
    // --- THÊM MỚI: Hàm vẽ hộp Code ---
    void drawCode(sf::RenderWindow& window);
    
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view);
    bool isDraggingNode() const;
};