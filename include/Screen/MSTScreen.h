#pragma once
#include "State.h"
#include "Button.h"
#include "InputBox.h"
#include "MultilineTextBox.h" // Khai báo class mới
#include "MSTGraph.h"

class App;

enum class UI_Mode { NONE, INPUT_GRAPH, RANDOM_GRAPH };

class MSTScreen : public State {
private:
    Button* btnBackToMenu;
    Button* btnInputGraph;
    Button* btnRandom;
    Button* btnKruskal;

    // UI Input Graph
    MultilineTextBox* inputGraphData; // Dùng class mới cho phép nhập nhiều dòng
    Button* btnEdgeList;
    Button* btnAdjMatrix;
    Button* btnAdjList;
    Button* btnGoInput;

    // Thêm vào phần private của MSTScreen:
    Button* btnStepBack;
    Button* btnPausePlay;
    Button* btnStepForward;
    Button* btnSpeedDown;
    Button* btnSpeedUp;

        // THÊM 2 NÚT NÀY (Nhảy về đầu và Nhảy tới cuối)
    Button* btnSkipBack;    
    Button* btnSkipForward;
        // THÊM DÒNG NÀY ĐỂ KHAI BÁO TEXT
    sf::Text textSpeed;
    int selectedInputType = 1;

    // UI Random Graph
    InputBox* inputV;
    InputBox* inputE;
    Button* btnGoRandom;

    UI_Mode currentMode = UI_Mode::NONE;
    MSTGraph* graph;

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