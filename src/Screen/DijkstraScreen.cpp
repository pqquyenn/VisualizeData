#include "DijkstraScreen.h"
#include "App.h"
#include "MainMenu.h"
#include <cmath>

DijkstraScreen::DijkstraScreen(App* app) : State(app) {
    btnBackToMenu = new Button(20, 20, 110, 40, app->font, "Back Menu");
    
    btnInputGraph = new Button(150, 80, 120, 40, app->font, "Input Graph");
    btnRandom = new Button(290, 80, 100, 40, app->font, "Random");
    btnMenuDijkstra = new Button(410, 80, 100, 40, app->font, "Dijkstra"); 

    inputGraphData = new MultilineTextBox(150, 140, 400, 200, app->font); 
    btnEdgeList = new Button(570, 140, 120, 30, app->font, "Edge List");
    btnAdjMatrix = new Button(570, 180, 120, 30, app->font, "Adj Matrix");
    btnAdjList = new Button(570, 220, 120, 30, app->font, "Adj List");
    btnGoInput = new Button(700, 220, 60, 30, app->font, "Go");

    btnStepBack    = new Button(530, 80, 40, 40, app->font, "<");          
    btnPausePlay   = new Button(590, 80, 120, 40, app->font, "Pause/Play"); 
    btnStepForward = new Button(730, 80, 40, 40, app->font, ">");          
    btnSpeedDown   = new Button(790, 80, 50, 40, app->font, "<<");         
    btnSpeedUp     = new Button(860, 80, 50, 40, app->font, ">>");         

    inputV = new InputBox(180, 140, 80, 40, app->font, 3); 
    inputE = new InputBox(310, 140, 80, 40, app->font, 3);
    btnGoRandom = new Button(410, 140, 60, 40, app->font, "Go");

    // UI Mới cho việc nhập Source Node
    inputSourceNode = new InputBox(410, 140, 60, 40, app->font, 3);
    btnRunDijkstraAction = new Button(480, 140, 60, 40, app->font, "Run");

    graph = new DijkstraGraph(app->font);
}

DijkstraScreen::~DijkstraScreen() {
    delete btnBackToMenu; delete btnInputGraph; delete btnRandom; delete btnMenuDijkstra;
    delete inputGraphData; delete btnEdgeList; delete btnAdjMatrix; delete btnAdjList; delete btnGoInput;
    delete inputV; delete inputE; delete btnGoRandom;
    delete inputSourceNode; delete btnRunDijkstraAction;
    delete graph;

    delete btnStepBack; delete btnPausePlay; delete btnStepForward;
    delete btnSpeedDown; delete btnSpeedUp;
}

void DijkstraScreen::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (!isViewInitialized) {
        graphView = window.getDefaultView();
        isViewInitialized = true;
    }

    if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
        if (event.mouseWheelScroll.delta > 0) graphView.zoom(0.9f);
        else if (event.mouseWheelScroll.delta < 0) graphView.zoom(1.1f);
    }

    window.setView(graphView);
    graph->handleEvent(event, window, graphView);

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        if (pixelPos.y > 350 && !graph->isDraggingNode()) { 
            isPanning = true; oldMousePos = pixelPos; 
        }
    } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        isPanning = false;
    } else if (event.type == sf::Event::MouseMoved && isPanning) {
        sf::Vector2i newMousePos = sf::Mouse::getPosition(window);
        sf::Vector2f delta = window.mapPixelToCoords(oldMousePos, graphView) - window.mapPixelToCoords(newMousePos, graphView);
        graphView.move(delta);
        oldMousePos = newMousePos;
    }

    window.setView(window.getDefaultView());
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (btnBackToMenu->isClicked(event, mousePos)) {
        app->changeState(new MainMenu(app)); return;
    }

    // Xử lý bật/tắt khung Input Graph
if (btnInputGraph->isClicked(event, mousePos)) {
        if (currentMode == Dijkstra_UI_Mode::INPUT_GRAPH) {
            currentMode = Dijkstra_UI_Mode::NONE; 
        } else {
            currentMode = Dijkstra_UI_Mode::INPUT_GRAPH; 
            showSourceInput = false; // Thêm dòng này để tắt khung Dijkstra nếu nó đang mở
        }
    }
    // Xử lý bật/tắt khung Random (Khuyên dùng để đồng bộ UX)
    if (btnRandom->isClicked(event, mousePos)) {
        if (currentMode == Dijkstra_UI_Mode::RANDOM_GRAPH) {
            currentMode = Dijkstra_UI_Mode::NONE; // Nếu đang mở thì tắt đi
        } else {
            currentMode = Dijkstra_UI_Mode::RANDOM_GRAPH; // Nếu đang tắt hoặc ở mode khác thì mở lên
            showSourceInput = false;
        }
    }
    
    // Khi bấm nút Dijkstra trên Menu, bật hộp nhập Node nguồn
// Xử lý bật/tắt khung nhập Source Node cho thuật toán Dijkstra
    if (btnMenuDijkstra->isClicked(event, mousePos)) {
        if (showSourceInput) {
            showSourceInput = false; // Nếu đang mở thì tắt đi
        } else {
            showSourceInput = true;  // Nếu đang tắt thì bật lên
            currentMode = Dijkstra_UI_Mode::NONE; // Đồng thời ẩn các khung Input/Random khác để tránh đè giao diện
        }
    }


    if (currentMode == Dijkstra_UI_Mode::INPUT_GRAPH) {
        inputGraphData->handleEvent(event, mousePos);
        if (btnEdgeList->isClicked(event, mousePos)) selectedInputType = 1;
        if (btnAdjMatrix->isClicked(event, mousePos)) selectedInputType = 2;
        if (btnAdjList->isClicked(event, mousePos)) selectedInputType = 3;

        if (btnGoInput->isClicked(event, mousePos)) {
            std::string data = inputGraphData->getText();
            if (!data.empty()) {
                if (selectedInputType == 1) graph->buildFromEdgeList(data);
                else if (selectedInputType == 2) {
                    std::stringstream ss(data); int temp, count = 0;
                    while(ss >> temp) count++;
                    int vCount = std::round(std::sqrt(count));
                    graph->buildFromAdjMatrix(data, vCount);
                }
                else if (selectedInputType == 3) graph->buildFromAdjList(data);
            }
        }
    } 
    else if (currentMode == Dijkstra_UI_Mode::RANDOM_GRAPH) {
        inputV->handleEvent(event, mousePos);
        inputE->handleEvent(event, mousePos);
        if (btnGoRandom->isClicked(event, mousePos)) {
            std::string vStr = inputV->getText();
            std::string eStr = inputE->getText();
            if (!vStr.empty() && !eStr.empty()) {
                try { graph->generateRandomGraph(std::stoi(vStr), std::stoi(eStr)); } catch (...) {}
            }
        }
    }

    // Xử lý khi chạy thuật toán
    if (showSourceInput) {
        inputSourceNode->handleEvent(event, mousePos);
        if (btnRunDijkstraAction->isClicked(event, mousePos)) {
            std::string sourceStr = inputSourceNode->getText();
            if (!sourceStr.empty()) {
                try {
                    int sourceId = std::stoi(sourceStr);
                    graph->startDijkstra(sourceId);
                } catch(...) {}
            }
        }
    }

    if (btnPausePlay->isClicked(event, mousePos)) graph->togglePause();
    if (btnStepBack->isClicked(event, mousePos)) graph->stepBackward();
    if (btnStepForward->isClicked(event, mousePos)) graph->stepForward();
    if (btnSpeedDown->isClicked(event, mousePos)) graph->decreaseSpeed();
    if (btnSpeedUp->isClicked(event, mousePos)) graph->increaseSpeed();
}

void DijkstraScreen::update(float deltaTime, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    btnBackToMenu->update(mousePos); btnInputGraph->update(mousePos); 
    btnRandom->update(mousePos); btnMenuDijkstra->update(mousePos);

    if (currentMode == Dijkstra_UI_Mode::INPUT_GRAPH) {
        btnEdgeList->update(mousePos); btnAdjMatrix->update(mousePos); 
        btnAdjList->update(mousePos); btnGoInput->update(mousePos);
    } else if (currentMode == Dijkstra_UI_Mode::RANDOM_GRAPH) {
        btnGoRandom->update(mousePos);
    }

    if (showSourceInput) {
        btnRunDijkstraAction->update(mousePos);
    }

    btnStepBack->update(mousePos); btnPausePlay->update(mousePos); 
    btnStepForward->update(mousePos); btnSpeedDown->update(mousePos); btnSpeedUp->update(mousePos);
    graph->update(deltaTime);
}

void DijkstraScreen::draw(sf::RenderWindow& window) {
    if (isViewInitialized) window.setView(graphView);
    graph->draw(window);
    window.setView(window.getDefaultView()); 
    
    btnBackToMenu->draw(window); 
    btnInputGraph->draw(window); btnRandom->draw(window); btnMenuDijkstra->draw(window);

    if (currentMode == Dijkstra_UI_Mode::INPUT_GRAPH) {
        inputGraphData->draw(window);
        btnEdgeList->draw(window); btnAdjMatrix->draw(window); 
        btnAdjList->draw(window); btnGoInput->draw(window);
        
        sf::Text hint; hint.setFont(app->font); hint.setCharacterSize(14); hint.setFillColor(sf::Color::White);
        hint.setPosition(150, 350);
        if (selectedInputType == 1) hint.setString("Format: u v weight (e.g. 1 2 99)");
        else if (selectedInputType == 2) hint.setString("Format: N x N grid (0-based indexing)");
        else if (selectedInputType == 3) hint.setString("Format: u v1 w1 v2 w2... (e.g. 4 2 99 3 4)");
        window.draw(hint);
    } 
    else if (currentMode == Dijkstra_UI_Mode::RANDOM_GRAPH) {
        inputV->draw(window); inputE->draw(window); btnGoRandom->draw(window);
        sf::Text labelV; labelV.setFont(app->font); labelV.setString("V:"); labelV.setCharacterSize(18); labelV.setPosition(150, 150);
        sf::Text labelE; labelE.setFont(app->font); labelE.setString("E:"); labelE.setCharacterSize(18); labelE.setPosition(280, 150);
        window.draw(labelV); window.draw(labelE);
    }

    if (showSourceInput) {
        inputSourceNode->draw(window);
        btnRunDijkstraAction->draw(window);
        sf::Text labelSource; labelSource.setFont(app->font); labelSource.setString("Source:"); 
        labelSource.setCharacterSize(18); labelSource.setPosition(340, 150); labelSource.setFillColor(sf::Color::White);
        window.draw(labelSource);
    }
    
    btnStepBack->draw(window); btnPausePlay->draw(window); 
    btnStepForward->draw(window); btnSpeedDown->draw(window); btnSpeedUp->draw(window);
}