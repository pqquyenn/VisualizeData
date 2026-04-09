#pragma once
#include "State.h"
#include "Button.h"
#include "InputBox.h"
#include "MSTGraph.h"

class App;

enum class UI_Mode { NONE, INPUT_GRAPH, RANDOM_GRAPH };

class MSTScreen : public State {
private:
    // Nút điều hướng chính
    Button* btnBackToMenu;
    Button* btnInputGraph;
    Button* btnRandom;
    Button* btnKruskal;

    // UI cho chế độ Input Graph
    InputBox* inputGraphData; // Giả sử dùng để nhập text. Nếu dài, bạn có thể tạo khung trắng lớn riêng
    Button* btnEdgeList;
    Button* btnAdjMatrix;
    Button* btnAdjList;
    Button* btnGoInput;
    int selectedInputType = 1; // 1: Edge List, 2: Adj Matrix, 3: Adj List

    // UI cho chế độ Random Graph
    InputBox* inputV;
    InputBox* inputE;
    Button* btnGoRandom;

    UI_Mode currentMode = UI_Mode::NONE;
    MSTGraph* graph;

    // Camera view
    sf::View graphView;
    bool isPanning = false;
    sf::Vector2i oldMousePos;
    bool isViewInitialized = false;

public:
    MSTScreen(App* app);
    ~MSTScreen();

    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
};