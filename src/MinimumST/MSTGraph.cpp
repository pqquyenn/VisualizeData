#include "MSTGraph.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "MSTGraph.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

MSTGraph::MSTGraph(sf::Font& font) : font(font) {
    speedMultiplier = 1.0f;
    std::srand(std::time(0));

    // --- THÊM: KHỞI TẠO GIAO DIỆN CODE ---
    codeBox.setFillColor(sf::Color(253, 246, 227)); 
    codeBox.setOutlineThickness(1.f);
    codeBox.setOutlineColor(sf::Color::Black);

    codeText.setFont(font);
    codeText.setCharacterSize(15);
    codeText.setFillColor(sf::Color::Black);
}

MSTGraph::~MSTGraph() { clear(); }

void MSTGraph::clear() {
    for (auto& pair : nodes) delete pair.second;
    nodes.clear();
    edges.clear();

    animationSteps.clear();
    currentStep = 0;
    timer = 0.0f;   
    isPaused = false;
    isAnimating = false;

    // --- THÊM: Xoá lịch sử code khi clear đồ thị ---
    currentCode.clear();
    currentHighlight = -1;
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
    // Vẽ cạnh (Line dày hơn)
    for (const auto& edge : edges) {
        sf::Vector2f posU = nodes[edge.u]->currentPos;
        sf::Vector2f posV = nodes[edge.v]->currentPos;
        
        // 1. Tính toán khoảng cách (độ dài cạnh) và góc xoay
        float dx = posV.x - posU.x;
        float dy = posV.y - posU.y;
        float length = std::sqrt(dx * dx + dy * dy);
        float angle = std::atan2(dy, dx) * 180.f / 3.14159265f; // Đổi từ radian sang độ

        // 2. Tạo hình chữ nhật đại diện cho cạnh
        float lineThickness = 4.0f; // TÙY CHỈNH ĐỘ DÀY Ở ĐÂY (4.0f là khá dễ nhìn)
        sf::RectangleShape thickLine(sf::Vector2f(length, lineThickness));
        
        // 3. Đặt tâm xoay ở chính giữa điểm đầu (posU)
        thickLine.setOrigin(0.f, lineThickness / 2.0f);
        thickLine.setPosition(posU);
        thickLine.setRotation(angle);
        thickLine.setFillColor(edge.color);

        window.draw(thickLine);

        // Vẽ trọng số (Weight)
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

    // Vẽ đỉnh (Vẽ sau để đỉnh đè lên trên các cạnh)
    for (auto& pair : nodes) pair.second->draw(window);
}
// Thêm vào cuối file MSTGraph.cpp của bạn

void MSTGraph::startKruskal() {
    if (edges.empty() || nodes.empty()) return;

    animationSteps.clear();
    currentStep = 0;
    timer = 0.0f;
    isPaused = false;
    isAnimating = true;

    for (auto& e : edges) e.color = sf::Color(200, 200, 200);

    // THÊM: Định nghĩa khối code thuật toán Kruskal
    std::vector<std::string> code = {
        "sort(edges.begin(), edges.end(), cmpWeight);",       // 0
        "for (int i = 0; i < V; ++i) makeSet(i);",            // 1
        "for (auto& edge : edges) {",                         // 2
        "    int rootU = find(edge.u);",                      // 3
        "    int rootV = find(edge.v);",                      // 4
        "    if (rootU != rootV) {",                          // 5
        "        unionSet(rootU, rootV);",                    // 6
        "        addToMST(edge);",                            // 7
        "    } else {",                                       // 8
        "        ignore(edge);",                              // 9
        "    }",                                              // 10
        "}"                                                   // 11
    };

    MSTState state;
    state.edgeColors.assign(edges.size(), sf::Color(200, 200, 200));
    state.codeLines = code;

    // Bước 0: Sắp xếp cạnh
    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.weight < b.weight;
    });
    state.highlightLine = 0;
    animationSteps.push_back(state);

    // Bước 1: Khởi tạo Disjoint Set
    DisjointSet ds;
    for (auto const& [id, node] : nodes) ds.makeSet(id);
    state.highlightLine = 1;
    animationSteps.push_back(state);

    for (size_t i = 0; i < edges.size(); ++i) {
        // Bước 2: Vòng lặp for
        state.highlightLine = 2;
        animationSteps.push_back(state);

        // Đang xét cạnh này (Đổi màu Vàng)
        state.edgeColors[i] = sf::Color::Yellow;
        
        // Bước 3 & 4: Tìm root
        state.highlightLine = 3;
        animationSteps.push_back(state);
        state.highlightLine = 4;
        animationSteps.push_back(state);

        int rootU = ds.find(edges[i].u);
        int rootV = ds.find(edges[i].v);

        // Bước 5: Kiểm tra root
        state.highlightLine = 5;
        animationSteps.push_back(state);

        if (rootU != rootV) {
            ds.unionSet(rootU, rootV);
            
            // Bước 6: Union
            state.highlightLine = 6;
            animationSteps.push_back(state);

            // Bước 7: Thêm vào MST (Đổi màu Xanh lá)
            state.edgeColors[i] = sf::Color::Green;
            state.highlightLine = 7;
            animationSteps.push_back(state);
        } else {
            // Bước 8: Else (Tạo chu trình)
            state.highlightLine = 8;
            animationSteps.push_back(state);

            // Bước 9: Ignore (Bỏ qua, màu xám đậm)
            state.edgeColors[i] = sf::Color(100, 100, 100); 
            state.highlightLine = 9;
            animationSteps.push_back(state);
        }
    }
    
    // Kết thúc thuật toán
    state.highlightLine = -1;
    animationSteps.push_back(state);
}

// --- CÁC HÀM ĐIỀU KHIỂN PLAYBACK ---
// --- CÁC HÀM ĐIỀU KHIỂN PLAYBACK ---
void MSTGraph::togglePause() { 
    isPaused = !isPaused; 
}

void MSTGraph::stepForward() {
    if (animationSteps.empty()) return;
    
    // Tự động Pause và reset bộ đếm thời gian khi người dùng tự tua
    isPaused = true; 
    timer = 0.0f; 
    
    if (currentStep < animationSteps.size() - 1) {
        currentStep++;
    }
}

void MSTGraph::stepBackward() {
    if (animationSteps.empty()) return;
    
    // Tự động Pause và reset bộ đếm thời gian khi người dùng tự tua
    isPaused = true; 
    timer = 0.0f;
    
    if (currentStep > 0) {
        currentStep--;
    }
}

void MSTGraph::skipToFirstStep() {
    if (animationSteps.empty()) return;
    
    isPaused = true;  // Tự động Pause thuật toán
    timer = 0.0f;     // Reset bộ đếm thời gian
    
    currentStep = 0;  // Nhảy về bước đầu tiên
    // Không cần hàm applyStep vì hàm update() của đồ thị đã tự động render theo currentStep
}

void MSTGraph::skipToLastStep() {
    if (animationSteps.empty()) return;
    
    isPaused = true;  // Tự động Pause thuật toán
    timer = 0.0f;     // Reset bộ đếm thời gian
    
    currentStep = animationSteps.size() - 1; // Nhảy tới bước cuối cùng
}
void MSTGraph::increaseSpeed() { 
    speedMultiplier += 0.25f;
    if (speedMultiplier > 3.0f) speedMultiplier = 3.0f; // Chặn tối đa ở mức 3.0x
    
    stepDuration = 0.8f / speedMultiplier; // Tính lại delay dựa trên hệ số tốc độ
}

// --- THÊM MỚI: HÀM DRAW CODE ---
void MSTGraph::drawCode(sf::RenderWindow& window) {
    if (currentCode.empty()) return;

    float boxWidth = 400.f; 
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

void MSTGraph::decreaseSpeed() { 
    speedMultiplier -= 0.25f;
    if (speedMultiplier < 0.25f) speedMultiplier = 0.25f; // Chặn tối thiểu ở mức 0.25x
    
    stepDuration = 0.8f / speedMultiplier; // Tính lại delay dựa trên hệ số tốc độ
}
// --- CẬP NHẬT HÀM UPDATE ---
// --- CẬP NHẬT HÀM UPDATE ---
// --- CẬP NHẬT HÀM UPDATE ---
void MSTGraph::update(float dt) {
    for (auto& pair : nodes) pair.second->update(dt);

    if (isAnimating && !isPaused) {
        timer += dt;
        if (timer >= stepDuration) {
            timer = 0.0f;
            if (currentStep < animationSteps.size() - 1) {
                currentStep++;
            } else {
                isPaused = true; 
            }
        }
    }

    // --- CẬP NHẬT: Lấy cả màu và code từ struct MSTState ---
    if (!animationSteps.empty() && currentStep < animationSteps.size()) {
        const auto& state = animationSteps[currentStep];
        
        for (size_t i = 0; i < edges.size(); ++i) {
            edges[i].color = state.edgeColors[i];
        }

        currentCode = state.codeLines;
        currentHighlight = state.highlightLine;
    }
}