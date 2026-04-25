#include "DijkstraGraph.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <queue> // Cần để dùng priority_queue

DijkstraGraph::DijkstraGraph(sf::Font& font) : font(font) {
    // THÊM DÒNG NÀY ĐỂ KHỞI TẠO TỐC ĐỘ MẶC ĐỊNH LÀ 1.0x
    speedMultiplier = 1.0f;
    std::srand(std::time(0));
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

    // reset màu cạnh mặc định trước khi chạy
    for(auto& edge : edges) edge.color = sf::Color(200, 200, 200);

    std::map<int, int> dist;
    std::map<int, int> parentEdge;
    std::map<int, bool> visited;
    
    // Khởi tạo trạng thái ban đầu cho animation
    DijkstraState state;
    state.edgeColors.assign(edges.size(), sf::Color(200, 200, 200)); // Xám
    
    for (auto const& [id, node] : nodes) {
        dist[id] = -1; // -1 đại diện cho INF
        visited[id] = false;
        parentEdge[id] = -1;
        state.nodeColors[id] = sf::Color(70, 130, 180); // Xanh bích (mặc định)
        state.nodeCosts[id] = -1;
    }
    
    dist[sourceId] = 0;
    state.nodeCosts[sourceId] = 0;
    animationSteps.push_back(state); // Bước 0

    // Priority Queue: pair<distance, node_id>
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    pq.push({0, sourceId});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (dist[u] != -1 && d > dist[u]) continue; // Skip nếu đã tìm được đường ngắn hơn

        visited[u] = true;

        // 2. Highlight Node đang xét (Xanh lá)
        state.nodeColors[u] = sf::Color::Green;
        animationSteps.push_back(state);

        // 3. Loang ra các đỉnh kề
        for (size_t i = 0; i < edges.size(); ++i) {
            int v = -1;
            if (edges[i].u == u) v = edges[i].v;
            else if (edges[i].v == u) v = edges[i].u;

            if (v != -1 && !visited[v]) {
                // Đổi cạnh đang check sang Vàng
                sf::Color oldEdgeColor = state.edgeColors[i];
                state.edgeColors[i] = sf::Color::Yellow; // Cạnh đang check (Vàng)
                animationSteps.push_back(state);

                int weight = edges[i].weight;
                if (dist[v] == -1 || dist[u] + weight < dist[v]) {
                    // Relax cạnh
                    dist[v] = dist[u] + weight;
                    state.nodeCosts[v] = dist[v];
                    
                    // Nếu đã có cạnh cha trước đó, chuyển cạnh cũ về xám (hoặc màu gốc nếu là cây)
                    if (parentEdge[v] != -1) state.edgeColors[parentEdge[v]] = sf::Color(200, 200, 200);
                    parentEdge[v] = i;
                    
                    // Highlight cạnh đường đi ngắn nhất tạm thời (Đổi từ Đỏ sang Cam sáng)
                    //state.edgeColors[i] = sf::Color::Red; // Màu đỏ gốc - BỊ CHÓI MẮT
                    state.edgeColors[i] = sf::Color(255, 165, 0); // Màu Cam sáng (255, 165, 0)
                    animationSteps.push_back(state);

                    pq.push({dist[v], v});
                } else {
                    // Trả lại màu cũ nếu không chọn
                    state.edgeColors[i] = oldEdgeColor;
                }
            }
        }

        // 4. Duyệt xong u, Node và cạnh cha chuyển sang màu Cam
        //state.nodeColors[u] = sf::Color::Red; // Màu đỏ gốc - BỊ CHÓI MẮT
        state.nodeColors[u] = sf::Color(210, 105, 30); // Màu Cam sậm hơn (Sienna) để dễ phân biệt với cạnh
        if (parentEdge[u] != -1) {
            state.edgeColors[parentEdge[u]] = sf::Color(255, 165, 0); // Giữ màu Cam sáng cho cạnh cha
        }
        animationSteps.push_back(state);
    }
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

    // Luôn áp dụng trạng thái đồ thị theo currentStep
    if (!animationSteps.empty() && currentStep < animationSteps.size()) {
        const auto& state = animationSteps[currentStep];
        
        // Cập nhật màu cạnh
        for (size_t i = 0; i < edges.size(); ++i) {
            edges[i].color = state.edgeColors[i];
        }
        
        // Cập nhật màu và cost cho Node
        for (auto& pair : nodes) {
            int id = pair.first;
            pair.second->currentColor = state.nodeColors.at(id);
            pair.second->setCost(state.nodeCosts.at(id));
        }
    }
}