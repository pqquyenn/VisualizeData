#pragma once
#include "Heap/HeapNode.h"
#include <vector>
#include <SFML/Graphics.hpp>

// --- FILE: include/Heap/HeapTree.h ---

// Lưu trữ trạng thái hiển thị của một node tại một thời điểm
struct HeapNodeInfo {
    int value;
    sf::Vector2f pos;
    sf::Color color;
};

// Lưu trữ toàn bộ khung hình của 1 bước hoạt ảnh
struct HeapSnapshot {
    std::vector<HeapNodeInfo> nodes;
};

class HeapTree {
private:
    std::vector<int> heapData;          // Mảng dữ liệu Max Heap thực sự
    sf::Font& font;
    std::vector<HeapNode*> visualNodes; // Quản lý object SFML qua mảng vector

    // Animation & Playback
    std::vector<HeapSnapshot> snapshots;
    size_t currentStep;
    float timer;
    float delay;       // Tốc độ hoạt ảnh (delay càng thấp, tốc độ càng nhanh)
    bool isPaused;

    // Các hàm Helper Heap Logic (Tạo hoạt ảnh)
    void heapifyUp(int index);   // Thêm node: So sánh & swap lên
    void heapifyDown(int index); // Xóa gốc: So sánh & swap xuống
    void buildHeap();            // Xây dựng heap O(N) từ mảng ngẫu nhiên

    // Core Animation Builder
    // Tính toán layout cây nhị phân hoàn chỉnh từ mảng
    void calculateLayout(std::vector<HeapNodeInfo>& snapshotNodes);
    
    // Lưu các snapshot khác nhau cho hoạt ảnh
    void saveSnapshot(); 
    void saveSnapshotCompare(int idx1, int idx2); // Snapshot: 2 node so sánh màu vàng
    void saveSnapshotSwap(int idx1, int idx2);    // Snapshot: Sau khi swap dữ liệu, visual nodes di chuyển

    void applyStep(size_t stepIndex);

public:
    HeapTree(sf::Font& font);
    ~HeapTree();

    // --- SỬA: Chức năng Init theo số lượng N nhập vào ---
    void initTree(int n); // Xóa cây, tạo n node ngẫu nhiên, build heap

    void insertVal(int val); // Insert val -> heapifyUp animation
    void deleteMax(); // Xóa gốc -> swap node cuối -> heapifyDown animation
    void clear();

    // Playback Controls
    void togglePause() { isPaused = !isPaused; }
    void stepForward();
    void stepBackward();
    void increaseSpeed(); // Tăng tốc độ bằng cách giảm delay
    void decreaseSpeed(); // Giảm tốc độ bằng cách tăng delay

    void updatePosition(float deltaTime);  // Cập nhật vị trí mượt của các Node
    void updateAnimation(float deltaTime); // Chuyển step tự động theo delay
    void draw(sf::RenderWindow& window);

    // Chuyển sự kiện cho cây (kéo node)
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view);
};