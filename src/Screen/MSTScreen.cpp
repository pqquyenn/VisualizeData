#include "MSTScreen.h"
#include "App.h"
#include "MainMenu.h"
#include <cmath>

MSTScreen::MSTScreen(App* app) : State(app) {
    btnBackToMenu = new Button(20, 20, 100, 40, app->font, "Back Menu");
    
    // 3 nút chức năng chính
    btnInputGraph = new Button(150, 20, 120, 40, app->font, "Input Graph");
    btnRandom = new Button(290, 20, 100, 40, app->font, "Random");
    btnKruskal = new Button(410, 20, 100, 40, app->font, "Kruskal");

    // UI Input Graph (Hiện khung trắng to)
    inputGraphData = new InputBox(150, 80, 400, 100, app->font, 200); // Tăng kích thước để nhập nhiều text
    btnEdgeList = new Button(570, 80, 120, 30, app->font, "Edge List");
    btnAdjMatrix = new Button(570, 120, 120, 30, app->font, "Adj Matrix");
    btnAdjList = new Button(570, 160, 120, 30, app->font, "Adj List");
    btnGoInput = new Button(700, 160, 60, 30, app->font, "Go");

    // UI Random
    inputV = new InputBox(150, 80, 100, 40, app->font, 3); // Số đỉnh
    inputE = new InputBox(270, 80, 100, 40, app->font, 3); // Số cạnh
    btnGoRandom = new Button(390, 80, 60, 40, app->font, "Go");

    graph = new MSTGraph(app->font);
}

MSTScreen::~MSTScreen() {
    delete btnBackToMenu; delete btnInputGraph; delete btnRandom; delete btnKruskal;
    delete inputGraphData; delete btnEdgeList; delete btnAdjMatrix; delete btnAdjList; delete btnGoInput;
    delete inputV; delete inputE; delete btnGoRandom;
    delete graph;
}

void MSTScreen::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (!isViewInitialized) {
        graphView = window.getDefaultView();
        isViewInitialized = true;
    }

    // Zoom Camera
    if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
        if (event.mouseWheelScroll.delta > 0) graphView.zoom(0.9f);
        else if (event.mouseWheelScroll.delta < 0) graphView.zoom(1.1f);
    }

    window.setView(graphView);
    graph->handleEvent(event, window, graphView);

    // Pan Camera
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        if (pixelPos.y > 200 && !graph->isDraggingNode()) { 
            isPanning = true; 
            oldMousePos = pixelPos; 
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
        app->changeState(new MainMenu(app)); 
        return;
    }

    // Chuyển Mode UI
    if (btnInputGraph->isClicked(event, mousePos)) currentMode = UI_Mode::INPUT_GRAPH;
    if (btnRandom->isClicked(event, mousePos)) currentMode = UI_Mode::RANDOM_GRAPH;
    if (btnKruskal->isClicked(event, mousePos)) {
        // Todo: Chạy thuật toán Kruskal (bước sau)
    }

    // Xử lý sự kiện theo Mode
    if (currentMode == UI_Mode::INPUT_GRAPH) {
        inputGraphData->handleEvent(event, mousePos);
        
        // Highlight loại input được chọn (bạn có thể đổi màu nút trong class Button để trực quan hơn)
        if (btnEdgeList->isClicked(event, mousePos)) selectedInputType = 1;
        if (btnAdjMatrix->isClicked(event, mousePos)) selectedInputType = 2;
        if (btnAdjList->isClicked(event, mousePos)) selectedInputType = 3;

        if (btnGoInput->isClicked(event, mousePos)) {
            std::string data = inputGraphData->getText();
            if (!data.empty()) {
                if (selectedInputType == 1) graph->buildFromEdgeList(data);
                else if (selectedInputType == 2) {
                    // Cần biết số đỉnh cho Matrix, tạm thời đếm từ độ dài chuỗi hoặc nhập tay thêm.
                    // Để đơn giản, giả sử ma trận vuông, sqrt(số lượng phần tử) = số đỉnh.
                    std::stringstream ss(data); int temp, count = 0;
                    while(ss >> temp) count++;
                    int vCount = std::round(std::sqrt(count));
                    graph->buildFromAdjMatrix(data, vCount);
                }
                else if (selectedInputType == 3) graph->buildFromAdjList(data);
            }
        }
    } 
    else if (currentMode == UI_Mode::RANDOM_GRAPH) {
        inputV->handleEvent(event, mousePos);
        inputE->handleEvent(event, mousePos);
        
        if (btnGoRandom->isClicked(event, mousePos)) {
            std::string vStr = inputV->getText();
            std::string eStr = inputE->getText();
            if (!vStr.empty() && !eStr.empty()) {
                try {
                    int v = std::stoi(vStr);
                    int e = std::stoi(eStr);
                    graph->generateRandomGraph(v, e);
                } catch (...) {} // Ignore parse errors
            }
        }
    }
}

void MSTScreen::update(float deltaTime, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    btnBackToMenu->update(mousePos); btnInputGraph->update(mousePos); 
    btnRandom->update(mousePos); btnKruskal->update(mousePos);

    if (currentMode == UI_Mode::INPUT_GRAPH) {
        btnEdgeList->update(mousePos); btnAdjMatrix->update(mousePos); 
        btnAdjList->update(mousePos); btnGoInput->update(mousePos);
    } else if (currentMode == UI_Mode::RANDOM_GRAPH) {
        btnGoRandom->update(mousePos);
    }

    graph->update(deltaTime);
}

void MSTScreen::draw(sf::RenderWindow& window) {
    if (isViewInitialized) window.setView(graphView);
    graph->draw(window);

    window.setView(window.getDefaultView()); // Vẽ UI cố định
    
    // Thanh background che các node trôi lên trên
    sf::RectangleShape uiBg(sf::Vector2f(window.getSize().x, 200.f));
    uiBg.setFillColor(sf::Color(40, 44, 52, 220));
    window.draw(uiBg);

    btnBackToMenu->draw(window); 
    btnInputGraph->draw(window); btnRandom->draw(window); btnKruskal->draw(window);

    if (currentMode == UI_Mode::INPUT_GRAPH) {
        inputGraphData->draw(window);
        btnEdgeList->draw(window); btnAdjMatrix->draw(window); 
        btnAdjList->draw(window); btnGoInput->draw(window);
        
        // Vẽ thêm chữ hướng dẫn nhỏ
        sf::Text hint; hint.setFont(app->font); hint.setCharacterSize(14); hint.setFillColor(sf::Color::White);
        hint.setPosition(150, 185);
        if (selectedInputType == 1) hint.setString("Format: u v weight (e.g. 0 1 5)");
        else if (selectedInputType == 2) hint.setString("Format: N x N grid numbers");
        else if (selectedInputType == 3) hint.setString("Format: u numEdges v1 w1 v2 w2...");
        window.draw(hint);
    } 
    else if (currentMode == UI_Mode::RANDOM_GRAPH) {
        inputV->draw(window);
        inputE->draw(window);
        btnGoRandom->draw(window);

        sf::Text labelV; labelV.setFont(app->font); labelV.setString("V:"); labelV.setCharacterSize(18); labelV.setPosition(125, 90);
        sf::Text labelE; labelE.setFont(app->font); labelE.setString("E:"); labelE.setCharacterSize(18); labelE.setPosition(245, 90);
        window.draw(labelV); window.draw(labelE);
    }
}