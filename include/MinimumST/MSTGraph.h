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

class MSTGraph {
private:
    sf::Font& font;
    std::map<int, MSTNode*> nodes;
    std::vector<Edge> edges;

    // --- Biến cho Animation Kruskal ---
    std::vector<std::vector<sf::Color>> animationSteps; // Lưu trạng thái màu của các cạnh ở mỗi bước
    int currentStep = 0;
    float timer = 0.0f;
    float stepDuration = 1.0f; // Tốc độ mặc định (1 giây / bước)
    bool isPaused = false;
    bool isAnimating = false;

        // THÊM DÒNG NÀY VÀO TRONG PHẦN PRIVATE:
    float speedMultiplier;

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

    // --- Hàm điều khiển Kruskal và Playback ---
    void startKruskal();
    void togglePause();
    void stepForward();
    void stepBackward();
    void increaseSpeed();
    void decreaseSpeed();

        // XoÁ HÀM getDelay() CŨ, THAY BẰNG HÀM NÀY:
    float getSpeedMultiplier() const { return speedMultiplier; }

        // THÊM 2 DÒNG NÀY VÀO ĐÂY:
    void skipToFirstStep();
    void skipToLastStep();

    void update(float dt);
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view);
    
    bool isDraggingNode() const;
};