#include "MSTGraph.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

MSTGraph::MSTGraph(sf::Font& font) : font(font) {
    std::srand(std::time(0));
}

MSTGraph::~MSTGraph() { clear(); }

void MSTGraph::clear() {
    for (auto& pair : nodes) delete pair.second;
    nodes.clear();
    edges.clear();
}

void MSTGraph::addNode(int id) {
    if (nodes.find(id) == nodes.end()) {
        nodes[id] = new MSTNode(id, font);
    }
}

void MSTGraph::arrangeCircularLayout() {
    if (nodes.empty()) return;
    float radius = 250.f;
    float centerX = 600.f, centerY = 400.f;
    float angleStep = (2.0f * 3.14159f) / nodes.size();
    
    int i = 0;
    for (auto& pair : nodes) {
        float angle = i * angleStep;
        pair.second->currentPos = sf::Vector2f(centerX + radius * cos(angle), centerY + radius * sin(angle));
        i++;
    }
}

bool MSTGraph::buildFromEdgeList(const std::string& input) {
    clear();
    std::stringstream ss(input);
    int u, v, w;
    // Format mong đợi: u v weight
    while (ss >> u >> v >> w) {
        addNode(u); addNode(v);
        edges.push_back({u, v, w});
    }
    arrangeCircularLayout();
    return true;
}

bool MSTGraph::buildFromAdjMatrix(const std::string& input, int numVertices) {
    clear();
    for (int i = 0; i < numVertices; i++) addNode(i);
    
    std::stringstream ss(input);
    int w;
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (ss >> w) {
                if (w != 0 && i < j) { // Đồ thị vô hướng, chỉ lấy nửa trên
                    edges.push_back({i, j, w});
                }
            }
        }
    }
    arrangeCircularLayout();
    return true;
}

bool MSTGraph::buildFromAdjList(const std::string& input) {
    clear();
    std::stringstream ss(input);
    int u, numEdges, v, w;
    // Format giả định: đỉnh_u số_lượng_cạnh đỉnh_v1 trọng_số_w1 ...
    while (ss >> u >> numEdges) {
        addNode(u);
        for (int i = 0; i < numEdges; i++) {
            if (ss >> v >> w) {
                addNode(v);
                if (u < v) edges.push_back({u, v, w}); // Tránh trùng lặp
            }
        }
    }
    arrangeCircularLayout();
    return true;
}

void MSTGraph::generateRandomGraph(int vCount, int eCount) {
    clear();
    if (vCount <= 0) return;

    for (int i = 0; i < vCount; i++) addNode(i);

    int edgesAdded = 0;
    int maxEdges = vCount * (vCount - 1) / 2;
    if (eCount > maxEdges) eCount = maxEdges;

    while (edgesAdded < eCount) {
        int u = std::rand() % vCount;
        int v = std::rand() % vCount;
        if (u != v) {
            bool exists = false;
            for (const auto& edge : edges) {
                if ((edge.u == u && edge.v == v) || (edge.u == v && edge.v == u)) {
                    exists = true; break;
                }
            }
            if (!exists) {
                int w = std::rand() % 99 + 1; // Trọng số từ 1 đến 99
                edges.push_back({std::min(u, v), std::max(u, v), w});
                edgesAdded++;
            }
        }
    }
    arrangeCircularLayout();
}

void MSTGraph::handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view) {
    for (auto& pair : nodes) pair.second->handleEvent(event, window, view);
}

bool MSTGraph::isDraggingNode() const {
    for (const auto& pair : nodes) {
        if (pair.second->isDragging) return true;
    }
    return false;
}

void MSTGraph::update(float dt) {
    for (auto& pair : nodes) pair.second->update(dt);
}

void MSTGraph::draw(sf::RenderWindow& window) {
    // Vẽ cạnh (Line)
    for (const auto& edge : edges) {
        sf::Vector2f posU = nodes[edge.u]->currentPos;
        sf::Vector2f posV = nodes[edge.v]->currentPos;
        
        sf::Vertex line[] = {
            sf::Vertex(posU, edge.color),
            sf::Vertex(posV, edge.color)
        };
        window.draw(line, 2, sf::Lines);

        // Vẽ trọng số (Weight)
        sf::Text weightText;
        weightText.setFont(font);
        weightText.setString(std::to_string(edge.weight));
        weightText.setCharacterSize(16);
        weightText.setFillColor(sf::Color::Yellow); // Màu vàng cho nổi bật
        weightText.setPosition((posU.x + posV.x) / 2.0f, (posU.y + posV.y) / 2.0f - 10.f);
        window.draw(weightText);
    }

    // Vẽ đỉnh
    for (auto& pair : nodes) pair.second->draw(window);
}