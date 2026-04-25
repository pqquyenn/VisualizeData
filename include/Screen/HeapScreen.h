#pragma once
#include <SFML/Graphics.hpp>
#include "State.h"       // Kế thừa từ class State chung của bạn
#include "HeapTree.h"
#include "Button.h"      // Class Button của bạn
#include "InputBox.h"    // Class InputBox của bạn
#include "fstream"

class App; // Forward declaration

class HeapScreen : public State {
private:
    sf::Font font;
    HeapTree* heapTree;

    // Giao diện (UI)
    Button* btnBackToMenu;
    InputBox* inputVal;
    Button* btnInsert;
    Button* btnDeleteMax;
    Button* btnInit;

    // Cụm nút Playback
    Button* btnSpeedDown;
    Button* btnStepBack;
    Button* btnPausePlay;
    Button* btnStepForward;
    Button* btnSpeedUp;

    // THÊM 2 NÚT NÀY (Nhảy về đầu và Nhảy tới cuối)
    Button* btnSkipBack;    
    Button* btnSkipForward;

        // THÊM DÒNG NÀY ĐỂ KHAI BÁO TEXT
    sf::Text textSpeed;

    // Biến cho Camera (Pan & Zoom)
    sf::View treeView;
    sf::View uiView;
    bool isPanning;
    bool isViewInitialized;
    sf::Vector2i oldMousePos;

    // ... các biến cũ giữ nguyên
   // ... (các biến cũ giữ nguyên)
    Button* btnInitFromFile;
    Button* btnSearch; // Thêm nút Search

    // ... (các mảng codeInsert, codeDelete giữ nguyên)

    // Thêm mảng pseudo-code cho Search
    std::vector<std::string> codeSearch = {
        "for (int i = 0; i < heap.size(); ++i)",
        "    if (heap[i] == val)",
        "        return i;  // Found",
        "return -1;  // Not found"
    };

    // Mảng chứa Pseudo-code để in ra màn hình
    std::vector<std::string> codeInsert = {
        "heap.push_back(val)",
        "index = heap.size() - 1",
        "while (index > 0 && heap[index] > heap[parent])",
        "    swap(heap[index], heap[parent])",
        "    index = parent"
    };

    std::vector<std::string> codeDelete = {
        "swap(heap[0], heap[last])",
        "heap.pop_back()",
        "heapifyDown(0)",
        "    largest = max(index, left, right)",
        "    if (largest != index)",
        "        swap(heap[index], heap[largest])",
        "        index = largest"
    };

    // Thêm hàm vẽ khung code
    void drawCodeBlock(sf::RenderWindow& window);
// ...

public:
    HeapScreen(App* app);
    ~HeapScreen();

    // Override các hàm ảo (virtual) từ class State
    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
};