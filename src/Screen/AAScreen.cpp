#include "AAScreen.h"
#include "App.h"
#include "MainMenu.h"
#include <string>
#include <exception>

// THÊM 2 THƯ VIỆN NÀY:
#include <sstream>
#include <iomanip>

AAScreen::AAScreen(App* app) : State(app) {
    btnBackToMenu = new Button(20, 20, 110, 40, app->font, "Back Menu");
    inputVal = new InputBox(150, 80, 100, 40, app->font, 3);
    
    btnInsert = new Button(270, 80, 100, 40, app->font, "Insert");
    btnSearch = new Button(390, 80, 100, 40, app->font, "Search");
    btnDelete = new Button(510, 80, 100, 40, app->font, "Delete");
    btnInit = new Button(630, 80, 100, 40, app->font, "Init");
    btnInitFromFile = new Button(750, 80, 100, 40, app->font, "Init File");

    btnSpeedDown   = new Button(150, 140, 50, 40, app->font, "<<");
    btnSkipBack    = new Button(190 + 20, 140, 40, 40, app->font, "|<"); // Nút mới
    btnStepBack    = new Button(240 + 20, 140, 40, 40, app->font, "<");
    btnPausePlay   = new Button(290 + 20, 140, 120, 40, app->font, "Pause/Play");
    btnStepForward = new Button(420 + 20, 140, 40, 40, app->font, ">");
    btnSkipForward = new Button(470 + 20, 140, 40, 40, app->font, ">|"); // Nút mới
    btnSpeedUp     = new Button(520 + 20, 140, 50, 40, app->font, ">>");
    
    aaTree = new AATree(app->font);
    
}

AAScreen::~AAScreen() {
    delete btnBackToMenu; delete btnInsert; delete btnSearch; 
    delete btnDelete; delete btnInit; delete inputVal;
    delete btnStepBack; delete btnPausePlay; delete btnStepForward;
    delete btnSpeedDown; delete btnSpeedUp; delete btnInitFromFile;
    delete btnSkipBack; delete btnSkipForward;
    delete aaTree;
}

void AAScreen::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (!isViewInitialized) {
        treeView = window.getDefaultView();
        isViewInitialized = true;
    }

    if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
        if (event.mouseWheelScroll.delta > 0) treeView.zoom(0.9f);
        else if (event.mouseWheelScroll.delta < 0) treeView.zoom(1.1f);
    }

    window.setView(treeView);
    aaTree->handleEvent(event, window, treeView);

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        if (pixelPos.y > 200 && !aaTree->isDraggingNode()) { 
            isPanning = true; oldMousePos = pixelPos; 
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        isPanning = false;
    }
    else if (event.type == sf::Event::MouseMoved && isPanning) {
        sf::Vector2i newMousePos = sf::Mouse::getPosition(window);
        sf::Vector2f delta = window.mapPixelToCoords(oldMousePos, treeView) - window.mapPixelToCoords(newMousePos, treeView);
        treeView.move(delta);
        oldMousePos = newMousePos;
    }

    window.setView(window.getDefaultView());

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    if (btnInitFromFile->isClicked(event, mousePos)) {
        std::ifstream file("input.txt");
        if (!file.is_open()) file.open("../input.txt"); 

        if (file.is_open()) {
            std::vector<int> data; int val;
            while (file >> val) data.push_back(val);
            file.close();
            if (!data.empty()) aaTree->initFromFile(data);
        }
    }
    
    if (btnBackToMenu->isClicked(event, mousePos)) {
        app->changeState(new MainMenu(app)); return;
    }
    
    inputVal->handleEvent(event, mousePos); 

    if (btnPausePlay->isClicked(event, mousePos)) aaTree->togglePause();
    if (btnStepBack->isClicked(event, mousePos)) aaTree->stepBackward();
    if (btnStepForward->isClicked(event, mousePos)) aaTree->stepForward();
    if (btnSpeedDown->isClicked(event, mousePos)) aaTree->decreaseSpeed();
    if (btnSpeedUp->isClicked(event, mousePos)) aaTree->increaseSpeed();


    if (btnInsert->isClicked(event, mousePos) || btnSearch->isClicked(event, mousePos) || 
        btnDelete->isClicked(event, mousePos) || btnInit->isClicked(event, mousePos)) {
        std::string text = inputVal->getText();
        if (!text.empty()) {
            try {
                int value = std::stoi(text); 
                if (btnInsert->isClicked(event, mousePos)) aaTree->insertVal(value);
                else if (btnSearch->isClicked(event, mousePos)) aaTree->searchVal(value);
                else if (btnDelete->isClicked(event, mousePos)) aaTree->deleteVal(value); 
                else if (btnInit->isClicked(event, mousePos)) aaTree->initTree(value); 
                inputVal->clear();
            } catch (const std::exception& e) {
                inputVal->clear();
            }        
        }
    }
}

void AAScreen::update(float deltaTime, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    btnBackToMenu->update(mousePos); btnInsert->update(mousePos);
    btnSearch->update(mousePos); btnDelete->update(mousePos); btnInit->update(mousePos);
    btnStepBack->update(mousePos); btnPausePlay->update(mousePos); btnStepForward->update(mousePos);
    btnSpeedDown->update(mousePos); btnSpeedUp->update(mousePos); btnInitFromFile->update(mousePos);

    aaTree->updateAnimation(deltaTime);
    aaTree->updatePosition(deltaTime);
}

void AAScreen::draw(sf::RenderWindow& window) {
    if (isViewInitialized) window.setView(treeView);
    aaTree->draw(window);

    window.setView(window.getDefaultView()); 

    btnBackToMenu->draw(window); inputVal->draw(window); 
    btnInsert->draw(window); btnSearch->draw(window); btnDelete->draw(window); 
    btnInit->draw(window); btnInitFromFile->draw(window); 
    
    drawCodeBlock(window); 
    btnStepBack->draw(window); btnPausePlay->draw(window); btnStepForward->draw(window);
    btnSpeedDown->draw(window); btnSpeedUp->draw(window);
}

void AAScreen::drawCodeBlock(sf::RenderWindow& window) {
    std::string op = aaTree->getCurrentOperation();
    if (op != "Insert" && op != "Delete" && op != "Search") return;

    const std::vector<std::string>* codePtr = &codeInsert;
    if (op == "Delete") codePtr = &codeDelete;
    else if (op == "Search") codePtr = &codeSearch;

    int activeLine = aaTree->getCurrentActiveLine();

    float boxX = 850.0f; float boxY = 400.0f;
    float boxWidth = 380.0f; float boxHeight = codePtr->size() * 30.0f + 20.0f;

    sf::RectangleShape bg(sf::Vector2f(boxWidth, boxHeight));
    bg.setPosition(boxX, boxY);
    bg.setFillColor(sf::Color(253, 246, 227)); 
    bg.setOutlineThickness(2.0f);
    bg.setOutlineColor(sf::Color(200, 200, 200));
    window.draw(bg);

    for (size_t i = 0; i < codePtr->size(); ++i) {
        float lineY = boxY + 10.0f + i * 30.0f;
        if ((int)i == activeLine) {
            sf::RectangleShape highlight(sf::Vector2f(boxWidth, 30.0f));
            highlight.setPosition(boxX, lineY - 5.0f);
            highlight.setFillColor(sf::Color(255, 228, 181)); 
            window.draw(highlight);
        }

        sf::Text text;
        text.setFont(app->font); 
        text.setString((*codePtr)[i]);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::Black);
        text.setPosition(boxX + 15.0f, lineY);
        window.draw(text);
    }
}