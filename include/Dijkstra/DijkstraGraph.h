#pragma once
#include "DijkstraNode.h"
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

struct DijkstraEdge {
    int u, v, weight;
    sf::Color color = sf::Color(200, 200, 200); // Xám nhạt mặc định
};

class DijkstraGraph {
private:
    sf::Font& font;
    std::map<int, DijkstraNode*> nodes;
    std::vector<DijkstraEdge> edges;

    // Biến cho Animation (Chuẩn bị cho thuật toán)
    std::vector<std::vector<sf::Color>> animationSteps; 
    int currentStep = 0;
    float timer = 0.0f;
    float stepDuration = 1.0f; 
    bool isPaused = false;
    bool isAnimating = false;

    void addNode(int id);
    void arrangeCircularLayout();

public:
    DijkstraGraph(sf::Font& font);
    ~DijkstraGraph();

    void clear();
    // Các hàm input giống y hệt Kruskal (Đồ thị vô hướng)
    bool buildFromEdgeList(const std::string& input);
    bool buildFromAdjMatrix(const std::string& input, int numVertices);
    bool buildFromAdjList(const std::string& input);
    void generateRandomGraph(int v, int e);

    // --- HÀM CHẠY THUẬT TOÁN (CHỜ BẠN CODE LOGIC) ---
    void startDijkstra(int sourceId); 
    void resetAllCosts(); // Gọi hàm này trước khi chạy thuật toán

    // --- CÁC HÀM ĐIỀU KHIỂN PLAYBACK ---
    void togglePause();
    void stepForward();
    void stepBackward();
    void increaseSpeed();
    void decreaseSpeed();

    void update(float dt);
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view);
    
    bool isDraggingNode() const;
};