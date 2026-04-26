#pragma once
#include "DijkstraNode.h"
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

struct DijkstraEdge {
    int u, v, weight;
    sf::Color color = sf::Color(200, 200, 200); 
};

// --- Cập nhật cấu trúc trạng thái để lưu thêm Code ---
struct DijkstraState {
    std::vector<sf::Color> edgeColors;
    std::map<int, sf::Color> nodeColors;
    std::map<int, int> nodeCosts;
    
    // THÊM: Lưu snapshot của code
    std::vector<std::string> codeLines;
    int highlightLine = -1;
};

class DijkstraGraph {
private:
    sf::Font& font;
    std::map<int, DijkstraNode*> nodes;
    std::vector<DijkstraEdge> edges;

    std::vector<DijkstraState> animationSteps; 
    int currentStep = 0;
    float timer = 0.0f;
    float stepDuration = 1.0f; 
    bool isPaused = false;
    bool isAnimating = false;

    void addNode(int id);
    void arrangeCircularLayout();

    float speedMultiplier;

    // THÊM CÁC BIẾN CHO GIAO DIỆN HIỂN THỊ CODE
    sf::RectangleShape codeBox;
    sf::Text codeText;
    std::vector<std::string> currentCode;
    int currentHighlight = -1;

public:
    DijkstraGraph(sf::Font& font);
    ~DijkstraGraph();

    void clear();
    bool buildFromEdgeList(const std::string& input);
    bool buildFromAdjMatrix(const std::string& input, int numVertices);
    bool buildFromAdjList(const std::string& input);
    void generateRandomGraph(int v, int e);

    void startDijkstra(int sourceId); 
    void resetAllCosts(); 

    void togglePause();
    void stepForward();
    void stepBackward();
    void skipToFirstStep();
    void skipToLastStep();

    void increaseSpeed();
    void decreaseSpeed();
    float getSpeedMultiplier() const { return speedMultiplier; }
    
    void update(float dt);
    void draw(sf::RenderWindow& window);
    // THÊM HÀM DRAW CODE
    void drawCode(sf::RenderWindow& window); 
    
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view);
    bool isDraggingNode() const;
};