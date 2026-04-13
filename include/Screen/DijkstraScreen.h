#pragma once
#include "State.h"
#include "Button.h"
#include "InputBox.h"
#include "MultilineTextBox.h" 
#include "DijkstraGraph.h"

class App;

enum class Dijkstra_UI_Mode { NONE, INPUT_GRAPH, RANDOM_GRAPH };

class DijkstraScreen : public State {
private:
    Button* btnBackToMenu;
    Button* btnInputGraph;
    Button* btnRandom;
    Button* btnRunDijkstra; // Đổi từ btnKruskal

    // UI Input Graph
    MultilineTextBox* inputGraphData; 
    Button* btnEdgeList;
    Button* btnAdjMatrix;
    Button* btnAdjList;
    Button* btnGoInput;

    // Playback
    Button* btnStepBack;
    Button* btnPausePlay;
    Button* btnStepForward;
    Button* btnSpeedDown;
    Button* btnSpeedUp;
    int selectedInputType = 1;

    // UI Random Graph
    InputBox* inputV;
    InputBox* inputE;
    Button* btnGoRandom;

    Dijkstra_UI_Mode currentMode = Dijkstra_UI_Mode::NONE;
    DijkstraGraph* graph;

    sf::View graphView;
    bool isPanning = false;
    sf::Vector2i oldMousePos;
    bool isViewInitialized = false;

public:
    DijkstraScreen(App* app);
    ~DijkstraScreen();

    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
};