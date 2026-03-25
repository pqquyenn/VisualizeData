#pragma once
#include "State.h"
#include "Button.h"
#include "InputBox.h"
#include "Heap/HeapTree.h"

// --- FILE: include/Heap/HeapScreen.h ---
// Quản lý màn hình Max Heap, kết nối UI Buttons/InputBox với HeapTree logic.
// Tích hợp TreeView (Pan/Zoom/Drag) và UIView (UI cố định).

class App; // Forward declaration

class HeapScreen : public State {
private:
    Button* btnBackToMenu;
    Button* btnInsert;
    Button* btnDeleteMax;
    Button* btnInit;

    // Playback Controls (Thứ tự đồng bộ với AVL)
    Button* btnStepBack;
    Button* btnPausePlay;
    Button* btnStepForward;
    Button* btnSpeedDown;
    Button* btnSpeedUp;

    InputBox* inputVal;
    HeapTree* heapTree; 
    sf::Font font;

    // View Panning & Zooming
    sf::View treeView;
    sf::View uiView;
    bool isPanning;
    sf::Vector2i oldMousePos;
    bool isViewInitialized;

public:
    HeapScreen(App* app);
    ~HeapScreen();

    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
};