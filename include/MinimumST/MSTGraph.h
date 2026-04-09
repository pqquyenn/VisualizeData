#pragma once
#include "MSTNode.h"
#include <map>
#include <vector>
#include <string>
#include <sstream>

struct Edge {
    int u, v, weight;
    sf::Color color = sf::Color(200, 200, 200); // Màu xám nhạt mặc định
};

class MSTGraph {
private:
    sf::Font& font;
    std::map<int, MSTNode*> nodes;
    std::vector<Edge> edges;

    void addNode(int id);
    void arrangeCircularLayout(); // Xếp các node thành vòng tròn

public:
    MSTGraph(sf::Font& font);
    ~MSTGraph();

    void clear();

    // Các hàm parse chuỗi đầu vào
    bool buildFromEdgeList(const std::string& input);
    bool buildFromAdjMatrix(const std::string& input, int numVertices);
    bool buildFromAdjList(const std::string& input);
    
    // Hàm tạo ngẫu nhiên
    void generateRandomGraph(int v, int e);

    void update(float dt);
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view);
    
    bool isDraggingNode() const;
};