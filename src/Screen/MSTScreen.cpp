#include "MSTScreen.h"
#include "App.h"
#include "MainMenu.h"
#include <cmath>

MSTScreen::MSTScreen(App* app) : State(app) {
    // Menu hạ xuống Y = 20
    btnBackToMenu = new Button(20, 20, 110, 40, app->font, "Back Menu");
    
    // Các nút chức năng chính hạ xuống Y = 80 (Giống AVL)
    btnInputGraph = new Button(150, 80, 120, 40, app->font, "Input Graph");
    btnRandom = new Button(290, 80, 100, 40, app->font, "Random");
    btnKruskal = new Button(410, 80, 100, 40, app->font, "Kruskal");

    // UI Input Graph hạ xuống Y = 140
    inputGraphData = new MultilineTextBox(150, 140, 400, 200, app->font); 
    btnEdgeList = new Button(570, 140, 120, 30, app->font, "Edge List");
    btnAdjMatrix = new Button(570, 180, 120, 30, app->font, "Adj Matrix");
    btnAdjList = new Button(570, 220, 120, 30, app->font, "Adj List");
    btnGoInput = new Button(700, 220, 60, 30, app->font, "Go");

    // Đặt dưới đoạn khởi tạo btnKruskal
// Các nút Playback (Nối tiếp sau nút Kruskal)
    btnSpeedDown   = new Button(530, 80, 40, 40, app->font, "<<");
    btnSkipBack    = new Button(580, 80, 40, 40, app->font, "|<"); 
    btnStepBack    = new Button(630, 80, 40, 40, app->font, "<");
    btnPausePlay   = new Button(680, 80, 110, 40, app->font, "Pause/Play");
    btnStepForward = new Button(800, 80, 40, 40, app->font, ">");
    btnSkipForward = new Button(850, 80, 40, 40, app->font, ">|"); 
    btnSpeedUp     = new Button(900, 80, 40, 40, app->font, ">>");      // 840 + 20 = 860
// Bạn có thể chỉnh lại toạ độ Y (đang để 300) cho phù hợp với UI của bạn

    // UI Random hạ xuống Y = 140, fix toạ độ để không đè chữ V/E
    inputV = new InputBox(180, 140, 80, 40, app->font, 3); 
    inputE = new InputBox(310, 140, 80, 40, app->font, 3);
    btnGoRandom = new Button(410, 140, 60, 40, app->font, "Go");

    graph = new MSTGraph(app->font);
}

MSTScreen::~MSTScreen() {
    delete btnBackToMenu; delete btnInputGraph; delete btnRandom; delete btnKruskal;
    delete inputGraphData; delete btnEdgeList; delete btnAdjMatrix; delete btnAdjList; delete btnGoInput;
    delete inputV; delete inputE; delete btnGoRandom;
    delete graph;
    delete btnSkipBack; delete btnSkipForward;
    delete btnStepBack; delete btnPausePlay; delete btnStepForward;
    delete btnSpeedDown; delete btnSpeedUp;
    }

void MSTScreen::handleEvent(sf::Event& event, sf::RenderWindow& window) {
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

    // Kéo Camera (Chỉ cho phép kéo khi chuột ở dưới vùng UI (Y > 350) để tránh đụng chạm Textbox)
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        if (pixelPos.y > 350 && !graph->isDraggingNode()) { 
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

    // Xử lý bật/tắt khung Input Graph
    if (btnInputGraph->isClicked(event, mousePos)) {
        if (currentMode == UI_Mode::INPUT_GRAPH) {
            currentMode = UI_Mode::NONE; // Nếu đang mở thì tắt đi
        } else {
            currentMode = UI_Mode::INPUT_GRAPH; // Nếu đang tắt hoặc ở mode khác thì mở lên
        }
    }
    // Xử lý bật/tắt khung Random (Khuyên dùng để đồng bộ UX)
    if (btnRandom->isClicked(event, mousePos)) {
        if (currentMode == UI_Mode::RANDOM_GRAPH) {
            currentMode = UI_Mode::NONE; // Nếu đang mở thì tắt đi
        } else {
            currentMode = UI_Mode::RANDOM_GRAPH; // Nếu đang tắt hoặc ở mode khác thì mở lên
        }
    }
    
    if (currentMode == UI_Mode::INPUT_GRAPH) {
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
                } catch (...) {}
            }
        }
    }

        // Thêm đoạn này vào phần xử lý button (bên dưới đoạn kiểm tra các nút Input/Random)

    if (btnKruskal->isClicked(event, mousePos)) {
        graph->startKruskal();
    }

    if (btnPausePlay->isClicked(event, mousePos)) graph->togglePause();
    if (btnSkipBack->isClicked(event, mousePos)) graph->skipToFirstStep();       // THÊM MỚI
    if (btnStepBack->isClicked(event, mousePos)) graph->stepBackward();
    if (btnStepForward->isClicked(event, mousePos)) graph->stepForward();
    if (btnSkipForward->isClicked(event, mousePos)) graph->skipToLastStep();
    if (btnSpeedDown->isClicked(event, mousePos)) graph->decreaseSpeed();
    if (btnSpeedUp->isClicked(event, mousePos)) graph->increaseSpeed();
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
    btnStepBack->update(mousePos); btnPausePlay->update(mousePos); 
    btnStepForward->update(mousePos); btnSpeedDown->update(mousePos); btnSpeedUp->update(mousePos);
    graph->update(deltaTime);
}

void MSTScreen::draw(sf::RenderWindow& window) {
    if (isViewInitialized) window.setView(graphView);
    graph->draw(window);

    window.setView(window.getDefaultView()); 
    
    // ĐÃ XÓA KHỐI VẼ NỀN ĐEN uiBg ĐỂ NODE KHÔNG BỊ TỐI

    btnBackToMenu->draw(window); 
    btnInputGraph->draw(window); btnRandom->draw(window); btnKruskal->draw(window);

    if (currentMode == UI_Mode::INPUT_GRAPH) {
        inputGraphData->draw(window);
        btnEdgeList->draw(window); btnAdjMatrix->draw(window); 
        btnAdjList->draw(window); btnGoInput->draw(window);
        
        sf::Text hint; hint.setFont(app->font); hint.setCharacterSize(14); hint.setFillColor(sf::Color::White);
        hint.setPosition(150, 350); // Dời Hint xuống dưới khung text box
       if (selectedInputType == 1) hint.setString("Format: n following lines input u v weight (e.g. 1 2 99)");
        else if (selectedInputType == 2) hint.setString("Format: N x N grid (0-based indexing)");
        else if (selectedInputType == 3) hint.setString("Format: First line u v1 w1 v2 w2... (e.g. 4 2 99 3 4)");
        window.draw(hint);
    } 
    else if (currentMode == UI_Mode::RANDOM_GRAPH) {
        inputV->draw(window);
        inputE->draw(window);
        btnGoRandom->draw(window);

        // Chỉnh lại toạ độ chữ V và E để nằm ngang hàng và không bị đè
        sf::Text labelV; labelV.setFont(app->font); labelV.setString("V:"); labelV.setCharacterSize(18); labelV.setPosition(150, 150);
        sf::Text labelE; labelE.setFont(app->font); labelE.setString("E:"); labelE.setCharacterSize(18); labelE.setPosition(280, 150);
        window.draw(labelV); window.draw(labelE);
    }
    btnStepBack->draw(window); btnPausePlay->draw(window); 
    btnStepForward->draw(window); btnSpeedDown->draw(window); btnSpeedUp->draw(window);
    btnSkipBack->draw(window); 
    btnSkipForward->draw(window);
}