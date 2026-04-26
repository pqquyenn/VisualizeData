#include "DijkstraGraph.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <queue> // Cần để dùng priority_queue

DijkstraGraph::DijkstraGraph(sf::Font& font) : font(font) {
    speedMultiplier = 1.0f;
    std::srand(std::time(0));

    // THÊM: KHỞI TẠO GIAO DIỆN CODE
    codeBox.setFillColor(sf::Color(253, 246, 227)); // Màu be sáng giống SLL
    codeBox.setOutlineThickness(1.f);
    codeBox.setOutlineColor(sf::Color::Black);

    codeText.setFont(font);
    codeText.setCharacterSize(15);
    codeText.setFillColor(sf::Color::Black);
}

DijkstraGraph::~DijkstraGraph() { clear(); }

void DijkstraGraph::clear() {
    for (auto& pair : nodes) delete pair.second;
    nodes.clear();
    edges.clear();

    animationSteps.clear();
    currentStep = 0;
    timer = 0.0f;   
    isPaused = false;
    isAnimating = false;

    // THÊM: Xoá lịch sử code khi clear đồ thị
    currentCode.clear();
    currentHighlight = -1;
}

void DijkstraGraph::addNode(int id) {
    if (nodes.find(id) == nodes.end()) {
        nodes[id] = new DijkstraNode(id, font);
    }
}

void DijkstraGraph::arrangeCircularLayout() {
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

void DijkstraGraph::resetAllCosts() {
    for(auto& pair : nodes) pair.second->resetCost();
}

bool DijkstraGraph::buildFromEdgeList(const std::string& input) {
    clear();
    std::stringstream ss(input);
    std::string line;
    while (std::getline(ss, line)) {
        std::stringstream lineStream(line);
        int u, v, w;
        if (lineStream >> u >> v >> w) {
            addNode(u); addNode(v);
            edges.push_back({u, v, w});
        }
    }
    arrangeCircularLayout();
    return true;
}

bool DijkstraGraph::buildFromAdjMatrix(const std::string& input, int numVertices) {
    clear();
    for (int i = 0; i < numVertices; i++) addNode(i);
    std::stringstream ss(input);
    std::vector<int> tokens;
    int temp;
    while (ss >> temp) tokens.push_back(temp);
    
    int startIndex = 0;
    if (tokens.size() == numVertices * numVertices + 1) startIndex = 1; 
    else if (tokens.size() > numVertices * numVertices) startIndex = tokens.size() - (numVertices * numVertices);

    int tokenIdx = startIndex;
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (tokenIdx < tokens.size()) {
                int w = tokens[tokenIdx++];
                if (w != 0 && i < j) edges.push_back({i, j, w});
            }
        }
    }
    arrangeCircularLayout();
    return true;
}

bool DijkstraGraph::buildFromAdjList(const std::string& input) {
    clear();
    std::stringstream ss(input);
    std::string line;
    while (std::getline(ss, line)) {
        std::stringstream lineStream(line);
        int u;
        if (lineStream >> u) {
            addNode(u);
            int v, w;
            while (lineStream >> v >> w) {
                addNode(v);
                if (u < v) edges.push_back({u, v, w}); 
            }
        }
    }
    arrangeCircularLayout();
    return true;
}

void DijkstraGraph::generateRandomGraph(int vCount, int eCount) {
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
                int w = std::rand() % 99 + 1; 
                edges.push_back({std::min(u, v), std::max(u, v), w});
                edgesAdded++;
            }
        }
    }
    arrangeCircularLayout();
}

void DijkstraGraph::handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view) {
    for (auto& pair : nodes) pair.second->handleEvent(event, window, view);
}

bool DijkstraGraph::isDraggingNode() const {
    for (const auto& pair : nodes) {
        if (pair.second->isDragging) return true;
    }
    return false;
}

void DijkstraGraph::drawCode(sf::RenderWindow& window) {
    if (currentCode.empty()) return;

    // Chiều rộng để 450.f vì code Dijkstra dài hơn SLL một chút
    float boxWidth = 450.f; 
    float lineHeight = 24.f;
    float boxHeight = currentCode.size() * lineHeight + 15.f; 
    
    sf::Vector2f viewSize = window.getDefaultView().getSize();
    float boxX = viewSize.x - boxWidth - 30.f;
    float boxY = viewSize.y - boxHeight - 30.f;

    codeBox.setSize(sf::Vector2f(boxWidth, boxHeight));
    codeBox.setPosition(boxX, boxY);
    window.draw(codeBox); 

    for (size_t i = 0; i < currentCode.size(); ++i) {
        float lineY = boxY + 7.f + i * lineHeight;

        if (static_cast<int>(i) == currentHighlight) {
            sf::RectangleShape highlightRect;
            highlightRect.setSize(sf::Vector2f(boxWidth, lineHeight));
            highlightRect.setPosition(boxX, lineY);
            highlightRect.setFillColor(sf::Color(255, 228, 181)); 
            window.draw(highlightRect);
        }

        codeText.setString(currentCode[i]);
        codeText.setPosition(boxX + 15.f, lineY + 2.f);
        window.draw(codeText);
    }
}

void DijkstraGraph::draw(sf::RenderWindow& window) {
    for (const auto& edge : edges) {
        sf::Vector2f posU = nodes[edge.u]->currentPos;
        sf::Vector2f posV = nodes[edge.v]->currentPos;
        
        float dx = posV.x - posU.x;
        float dy = posV.y - posU.y;
        float length = std::sqrt(dx * dx + dy * dy);
        float angle = std::atan2(dy, dx) * 180.f / 3.14159265f; 

        float lineThickness = 4.0f; 
        sf::RectangleShape thickLine(sf::Vector2f(length, lineThickness));
        
        thickLine.setOrigin(0.f, lineThickness / 2.0f);
        thickLine.setPosition(posU);
        thickLine.setRotation(angle);
        thickLine.setFillColor(edge.color);
        window.draw(thickLine);

// Trong file DijkstraGraph.cpp, hàm draw()
        sf::Text weightText;
        weightText.setFont(font);
        weightText.setString(std::to_string(edge.weight));

        weightText.setCharacterSize(18); // Tăng nhẹ từ 16 lên 18
        weightText.setStyle(sf::Text::Bold); // In đậm
        weightText.setFillColor(sf::Color::Yellow);
        weightText.setOutlineColor(sf::Color::Black); // Thêm viền đen
        weightText.setOutlineThickness(2.f); 

        weightText.setPosition((posU.x + posV.x) / 2.0f, (posU.y + posV.y) / 2.0f - 15.f);
        window.draw(weightText);
    }
    for (auto& pair : nodes) pair.second->draw(window);
}

// --- THUẬT TOÁN DIJKSTRA ĐÃ CHỈNH SỬA MÀU CAM ---
void DijkstraGraph::startDijkstra(int sourceId) {
    if (edges.empty() || nodes.empty()) return;
    if (nodes.find(sourceId) == nodes.end()) return;

    animationSteps.clear();
    currentStep = 0;
    timer = 0.0f;
    isPaused = false;
    isAnimating = true;

    for(auto& edge : edges) edge.color = sf::Color(200, 200, 200);

    // THÊM: Định nghĩa khối code thuật toán
    std::vector<std::string> code = {
        "dist[source] = 0; pq.push({0, source});",                    // 0
        "while (!pq.empty()) {",                                      // 1
        "    int u = pq.top().second; pq.pop();",                     // 2
        "    if (visited[u]) continue;",                              // 3
        "    visited[u] = true;",                                     // 4
        "    for (edge : adj[u]) {",                                  // 5
        "        int v = edge.to, w = edge.weight;",                  // 6
        "        if (!visited[v] && dist[u] + w < dist[v]) {",        // 7
        "            dist[v] = dist[u] + w;",                         // 8
        "            pq.push({dist[v], v});",                         // 9
        "        }",                                                  // 10
        "    }",                                                      // 11
        "}"                                                           // 12
    };

    std::map<int, int> dist;
    std::map<int, int> parentEdge;
    std::map<int, bool> visited;
    
    DijkstraState state;
    state.edgeColors.assign(edges.size(), sf::Color(200, 200, 200)); 
    state.codeLines = code; // Nạp code vào state
    
    for (auto const& [id, node] : nodes) {
        dist[id] = -1; 
        visited[id] = false;
        parentEdge[id] = -1;
        state.nodeColors[id] = sf::Color(70, 130, 180); 
        state.nodeCosts[id] = -1;
    }
    
    dist[sourceId] = 0;
    state.nodeCosts[sourceId] = 0;
    state.highlightLine = 0; // Highlight dòng 0
    animationSteps.push_back(state); 

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    pq.push({0, sourceId});

    while (!pq.empty()) {
        state.highlightLine = 1; // Highlight: while (!pq.empty())
        animationSteps.push_back(state);

        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        state.highlightLine = 2; // Highlight: pop
        animationSteps.push_back(state);

        state.highlightLine = 3; // Highlight: check visited
        if (dist[u] != -1 && d > dist[u]) {
            animationSteps.push_back(state);
            continue; 
        }

        visited[u] = true;
        state.nodeColors[u] = sf::Color::Green;
        state.highlightLine = 4; // Highlight: visited = true
        animationSteps.push_back(state);

        state.highlightLine = 5; // Highlight: for edges
        animationSteps.push_back(state);

        for (size_t i = 0; i < edges.size(); ++i) {
            int v = -1;
            if (edges[i].u == u) v = edges[i].v;
            else if (edges[i].v == u) v = edges[i].u;

            if (v != -1 && !visited[v]) {
                sf::Color oldEdgeColor = state.edgeColors[i];
                state.edgeColors[i] = sf::Color::Yellow; 
                state.highlightLine = 6; // Highlight: lấy cạnh
                animationSteps.push_back(state);

                int weight = edges[i].weight;
                state.highlightLine = 7; // Highlight: if relax
                animationSteps.push_back(state);

                if (dist[v] == -1 || dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    state.nodeCosts[v] = dist[v];
                    
                    if (parentEdge[v] != -1) state.edgeColors[parentEdge[v]] = sf::Color(200, 200, 200);
                    parentEdge[v] = i;
                    state.edgeColors[i] = sf::Color(255, 165, 0); 
                    
                    state.highlightLine = 8; // Highlight: cập nhật khoảng cách
                    animationSteps.push_back(state);

                    pq.push({dist[v], v});
                    state.highlightLine = 9; // Highlight: push pq
                    animationSteps.push_back(state);
                } else {
                    state.edgeColors[i] = oldEdgeColor;
                }
            }
        }

        state.nodeColors[u] = sf::Color(210, 105, 30); 
        if (parentEdge[u] != -1) {
            state.edgeColors[parentEdge[u]] = sf::Color(255, 165, 0); 
        }
        state.highlightLine = 12; // Highlight: End while
        animationSteps.push_back(state);
    }
    
    // Bước kết thúc: Xoá highlight
    state.highlightLine = -1;
    animationSteps.push_back(state);
}

void DijkstraGraph::togglePause() { isPaused = !isPaused; }
void DijkstraGraph::stepForward() {
    if (animationSteps.empty()) return;
    isPaused = true; timer = 0.0f; 
    if (currentStep < animationSteps.size() - 1) currentStep++;
}
void DijkstraGraph::stepBackward() {
    if (animationSteps.empty()) return;
    isPaused = true; timer = 0.0f;
    if (currentStep > 0) currentStep--;
}

// THÊM CODE NÀY VÀO TRONG FILE SinglyLinkedList.cpp
void DijkstraGraph::skipToFirstStep() {
    if (animationSteps.empty()) return;
    
    isPaused = true;  // Tự động Pause thuật toán
    timer = 0.0f;     // Reset bộ đếm thời gian
    
    currentStep = 0;  // Nhảy về bước đầu tiên
    // Không cần hàm applyStep vì hàm update() của đồ thị đã tự động render theo currentStep
}

void DijkstraGraph::skipToLastStep() {
    if (animationSteps.empty()) return;
    
    isPaused = true;  // Tự động Pause thuật toán
    timer = 0.0f;     // Reset bộ đếm thời gian
    
    currentStep = animationSteps.size() - 1; // Nhảy tới bước cuối cùng
}
void DijkstraGraph::increaseSpeed() { 
    speedMultiplier += 0.25f;
    if (speedMultiplier > 3.0f) speedMultiplier = 3.0f; // Chặn tối đa ở mức 3.0x
    
    stepDuration = 0.8f / speedMultiplier; // Tính lại delay dựa trên hệ số tốc độ
}

void DijkstraGraph::decreaseSpeed() { 
    speedMultiplier -= 0.25f;
    if (speedMultiplier < 0.25f) speedMultiplier = 0.25f; // Chặn tối thiểu ở mức 0.25x
    
    stepDuration = 0.8f / speedMultiplier; // Tính lại delay dựa trên hệ số tốc độ
}

void DijkstraGraph::update(float dt) {
    for (auto& pair : nodes) pair.second->update(dt);

    if (isAnimating && !isPaused) {
        timer += dt;
        if (timer >= stepDuration) {
            timer = 0.0f;
            if (currentStep < animationSteps.size() - 1) currentStep++;
            else isPaused = true; 
        }
    }

    if (!animationSteps.empty() && currentStep < animationSteps.size()) {
        const auto& state = animationSteps[currentStep];
        
        for (size_t i = 0; i < edges.size(); ++i) {
            edges[i].color = state.edgeColors[i];
        }
        
        for (auto& pair : nodes) {
            int id = pair.first;
            pair.second->currentColor = state.nodeColors.at(id);
            pair.second->setCost(state.nodeCosts.at(id));
        }

        // THÊM: Cập nhật dòng code hiện tại từ state
        currentCode = state.codeLines;
        currentHighlight = state.highlightLine;
    }
}