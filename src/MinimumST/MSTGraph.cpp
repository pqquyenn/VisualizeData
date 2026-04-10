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
    std::string line;
    
    // Đọc từng dòng
    while (std::getline(ss, line)) {
        std::stringstream lineStream(line);
        int u, v, w;
        
        // Chỉ nhận khi dòng đó có ĐÚNG 3 thông số (u, v, w)
        // Nếu dòng đầu là V E (ví dụ "5 7"), nó sẽ lướt qua để không bị lỗi.
        if (lineStream >> u >> v >> w) {
            addNode(u); addNode(v);
            edges.push_back({u, v, w});
        }
    }
    arrangeCircularLayout();
    return true;
}

bool MSTGraph::buildFromAdjMatrix(const std::string& input, int numVertices) {
    clear();
    
    // 1. TẠO ĐỈNH TỪ 0 ĐẾN numVertices - 1
    for (int i = 0; i < numVertices; i++) addNode(i);
    
    std::stringstream ss(input);
    std::vector<int> tokens;
    int temp;
    
    // Đọc toàn bộ các số có trong khung nhập liệu
    while (ss >> temp) {
        tokens.push_back(temp);
    }
    
    // 2. LỌC SỐ ĐỈNH 'N' Ở ĐẦU (ví dụ như số 5 ở dòng đầu tiên trong hình của bạn)
    int startIndex = 0;
    // Nếu tổng các số đọc được đúng bằng N*N + 1, nghĩa là số đầu tiên là số lượng đỉnh -> Cần bỏ qua
    if (tokens.size() == numVertices * numVertices + 1) {
        startIndex = 1; 
    } 
    // Nếu dư dả gì đó, ta cố gắng lùi lại lấy đúng N*N phần tử ở cuối
    else if (tokens.size() > numVertices * numVertices) {
        startIndex = tokens.size() - (numVertices * numVertices);
    }

    // 3. TIẾN HÀNH ĐỌC MA TRẬN KỀ
    int tokenIdx = startIndex;
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (tokenIdx < tokens.size()) {
                int w = tokens[tokenIdx++];
                // Nếu trọng số w != 0 nghĩa là có cạnh.
                // i < j để chỉ lấy nửa trên của ma trận (vì là đồ thị vô hướng, tránh vẽ 2 cạnh đè lên nhau)
                if (w != 0 && i < j) { 
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
    std::string line;
    
    // Đọc từng dòng
    while (std::getline(ss, line)) {
        std::stringstream lineStream(line);
        int u;
        // Số đầu tiên của dòng là đỉnh gốc U
        if (lineStream >> u) {
            addNode(u);
            int v, w;
            // Liên tục đọc các cặp V và Trọng lượng W đứng sau
            while (lineStream >> v >> w) {
                addNode(v);
                if (u < v) edges.push_back({u, v, w}); // Thêm điều kiện để không tạo cạnh đôi
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

// Thêm vào cuối file MSTGraph.cpp của bạn

void MSTGraph::startKruskal() {
    if (edges.empty() || nodes.empty()) return;

    // 1. Reset trạng thái
    animationSteps.clear();
    currentStep = 0;
    timer = 0.0f;
    isPaused = false;
    isAnimating = true;

    // Reset màu tất cả cạnh về xám
    for (auto& e : edges) e.color = sf::Color(200, 200, 200);

    // 2. Sắp xếp cạnh theo trọng số tăng dần (Cốt lõi của Kruskal)
    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.weight < b.weight;
    });

    // 3. Khởi tạo Union-Find
    DisjointSet ds;
    for (auto const& [id, node] : nodes) ds.makeSet(id);

    // Lưu bước 0: Trạng thái ban đầu sau khi đã sort cạnh
    std::vector<sf::Color> currentState(edges.size(), sf::Color(200, 200, 200));
    animationSteps.push_back(currentState);

    // 4. Chạy thuật toán và lưu từng bước
    for (size_t i = 0; i < edges.size(); ++i) {
        // Bước A: Đang xét cạnh này (Đổi màu Vàng)
        currentState[i] = sf::Color::Yellow;
        animationSteps.push_back(currentState);

        int rootU = ds.find(edges[i].u);
        int rootV = ds.find(edges[i].v);

        if (rootU != rootV) {
            // Bước B1: Không tạo chu trình -> Thêm vào MST (Đổi màu Xanh lá)
            ds.unionSet(rootU, rootV);
            currentState[i] = sf::Color::Green;
        } else {
            // Bước B2: Tạo chu trình -> Bỏ qua (Đổi màu Đỏ hoặc Xám đậm)
            currentState[i] = sf::Color(100, 100, 100); // Xám đậm
        }
        animationSteps.push_back(currentState);
    }
}

// --- CÁC HÀM ĐIỀU KHIỂN PLAYBACK ---
void MSTGraph::togglePause() { isPaused = !isPaused; }

void MSTGraph::stepForward() {
    if (isAnimating && currentStep < animationSteps.size() - 1) {
        currentStep++;
        timer = 0; // Reset timer để chờ người dùng tự bấm tiếp
    }
}

void MSTGraph::stepBackward() {
    if (isAnimating && currentStep > 0) {
        currentStep--;
        timer = 0;
    }
}

void MSTGraph::increaseSpeed() {
    stepDuration = std::max(0.1f, stepDuration - 0.2f); // Nhanh nhất là 0.1s/bước
}

void MSTGraph::decreaseSpeed() {
    stepDuration = std::min(3.0f, stepDuration + 0.2f); // Chậm nhất là 3s/bước
}

// --- CẬP NHẬT HÀM UPDATE ---
void MSTGraph::update(float dt) {
    for (auto& pair : nodes) pair.second->update(dt);

    // Xử lý Animation
    if (isAnimating && !isPaused) {
        timer += dt;
        if (timer >= stepDuration) {
            timer = 0;
            if (currentStep < animationSteps.size() - 1) {
                currentStep++;
            } else {
                isAnimating = false; // Xong thuật toán
            }
        }
    }

    // Áp dụng màu sắc cho cạnh dựa trên bước hiện tại
    if (isAnimating || currentStep == animationSteps.size() - 1) {
        if (!animationSteps.empty() && currentStep < animationSteps.size()) {
            for (size_t i = 0; i < edges.size(); ++i) {
                edges[i].color = animationSteps[currentStep][i];
            }
        }
    }
}