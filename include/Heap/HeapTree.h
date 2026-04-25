#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "HeapNode.h"

// Cấu trúc lưu thông tin 1 node tại 1 thời điểm (để làm animation)
struct HeapNodeInfo {
    int value;
    sf::Vector2f pos;
    sf::Color color;
};

// Cấu trúc lưu trạng thái toàn bộ cây tại 1 bước
struct HeapSnapshot {
    std::vector<HeapNodeInfo> nodes;
    std::string operation; // Tên thao tác: "Insert", "Delete", "Init"
    int activeLine;        // Dòng code đang được highlight (bắt đầu từ 0)
};

class HeapTree {
private:
    std::vector<int> heapData;                 // Mảng dữ liệu thực tế của Heap
    std::vector<HeapNode*> visualNodes;        // Các node để vẽ lên màn hình
    std::vector<HeapSnapshot> snapshots;       // Các bước hoạt ảnh
    
    sf::Font& font;
    size_t currentStep;
    float timer;
    float delay;
    bool isPaused;
    
    HeapNode* draggedNode; // Con trỏ lưu Node đang bị kéo thả

    void calculateLayout(std::vector<HeapNodeInfo>& snapshotNodes);
    // 2. Sửa lại tham số của các hàm lưu Snapshot
    void saveSnapshot(std::string op = "", int line = -1);
    void saveSnapshotCompare(int idx1, int idx2, std::string op = "", int line = -1);
    void saveSnapshotSwap(int idx1, int idx2, std::string op = "", int line = -1);
    void buildHeap();
    void heapifyUp(int index);
    void heapifyDown(int index);
    void applyStep(size_t stepIndex);


public:
    HeapTree(sf::Font& f);
    ~HeapTree();

    void clear();
    void initTree(int n);
    void insertVal(int val);
    void deleteMax();

    // Các tính năng điều khiển Playback
    void stepForward();
    void stepBackward();
    void increaseSpeed();
    void decreaseSpeed();
    // THÊM 2 DÒNG NÀY VÀO ĐÂY:
    void skipToFirstStep();
    void skipToLastStep();
    void togglePause() { isPaused = !isPaused; }

    // Xử lý đồ họa & Event
    void updateAnimation(float deltaTime);
    void updatePosition(float deltaTime);
    void draw(sf::RenderWindow& window);
    
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, const sf::View& view);
    bool isDraggingNode() const; // Kiểm tra xem có node nào đang bị kéo không

    // 3. Thêm hàm khởi tạo từ file
    void initFromFile(const std::vector<int>& data);

    // 4. Thêm getter để lấy thông tin code vẽ ra màn hình
    std::string getCurrentOperation() const;
    int getCurrentActiveLine() const;

    // Thêm hàm Search
    void searchVal(int val);
};