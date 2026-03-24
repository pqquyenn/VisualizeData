#include "AVLScreen.h"
#include "App.h"
#include "MainMenu.h"
#include <string>
#include <exception>

AVLScreen::AVLScreen(App* app) : State(app) {
    btnBackToMenu = new Button(20, 20, 100, 40, app->font, "Back Menu");
    inputVal = new InputBox(150, 80, 100, 40, app->font, 3);
    
    btnInsert = new Button(270, 80, 100, 40, app->font, "Insert");
    btnSearch = new Button(390, 80, 100, 40, app->font, "Search");
    btnDelete = new Button(510, 80, 100, 40, app->font, "Delete");
    btnInit = new Button(630, 80, 100, 40, app->font, "Init");

    // Playback Controls UI
    btnStepBack = new Button(200, 140, 40, 40, app->font, "<");
    btnPausePlay = new Button(250, 140, 120, 40, app->font, "Pause/Play");
    btnStepForward = new Button(380, 140, 40, 40, app->font, ">");
    btnSpeedDown = new Button(430, 140, 50, 40, app->font, "<<");
    btnSpeedUp = new Button(490, 140, 50, 40, app->font, ">>");

    avlTree = new AVLTree(app->font);
}

AVLScreen::~AVLScreen() {
    delete btnBackToMenu; delete btnInsert; delete btnSearch; 
    delete btnDelete; delete btnInit; delete inputVal;
    delete btnStepBack; delete btnPausePlay; delete btnStepForward;
    delete btnSpeedDown; delete btnSpeedUp;
    delete avlTree;
}

void AVLScreen::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (!isViewInitialized) {
        treeView = window.getDefaultView();
        isViewInitialized = true;
    }

    // Camera Panning & Zooming
    if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
        if (event.mouseWheelScroll.delta > 0) treeView.zoom(0.9f);
        else if (event.mouseWheelScroll.delta < 0) treeView.zoom(1.1f);
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        if (pixelPos.y > 200) { isPanning = true; oldMousePos = pixelPos; } // Tránh kéo nhầm nút bấm UI
    }
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) isPanning = false;
    
    if (event.type == sf::Event::MouseMoved && isPanning) {
        sf::Vector2i newMousePos = sf::Mouse::getPosition(window);
        sf::Vector2f delta = window.mapPixelToCoords(oldMousePos, treeView) - window.mapPixelToCoords(newMousePos, treeView);
        treeView.move(delta);
        oldMousePos = newMousePos;
    }

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    if (btnBackToMenu->isClicked(event, mousePos)) {
        app->changeState(new MainMenu(app)); 
        return;
    }
    
    inputVal->handleEvent(event, mousePos); 

    // Playback logic
    if (btnPausePlay->isClicked(event, mousePos)) avlTree->togglePause();
    if (btnStepBack->isClicked(event, mousePos)) avlTree->stepBackward();
    if (btnStepForward->isClicked(event, mousePos)) avlTree->stepForward();
    if (btnSpeedDown->isClicked(event, mousePos)) avlTree->decreaseSpeed();
    if (btnSpeedUp->isClicked(event, mousePos)) avlTree->increaseSpeed();

    // Data Structure Logic
    if (btnInsert->isClicked(event, mousePos) || btnSearch->isClicked(event, mousePos) || 
        btnDelete->isClicked(event, mousePos) || btnInit->isClicked(event, mousePos)) {
        
        std::string text = inputVal->getText();
        if (!text.empty()) {
            try {
                int value = std::stoi(text); 
                if (btnInsert->isClicked(event, mousePos)) avlTree->insertVal(value);
                else if (btnSearch->isClicked(event, mousePos)) avlTree->searchVal(value);
                else if (btnDelete->isClicked(event, mousePos)) avlTree->deleteVal(value); 
                else if (btnInit->isClicked(event, mousePos)) avlTree->initTree(value); 
                inputVal->clear();
            } catch (const std::exception& e) {
                inputVal->clear();
            }        
        }
    }
}

void AVLScreen::update(float deltaTime, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    btnBackToMenu->update(mousePos); btnInsert->update(mousePos);
    btnSearch->update(mousePos); btnDelete->update(mousePos); btnInit->update(mousePos);
    btnStepBack->update(mousePos); btnPausePlay->update(mousePos); btnStepForward->update(mousePos);
    btnSpeedDown->update(mousePos); btnSpeedUp->update(mousePos);

    avlTree->updateAnimation(deltaTime);
    avlTree->updatePosition(deltaTime);
}

void AVLScreen::draw(sf::RenderWindow& window) {
    if (isViewInitialized) window.setView(treeView);
    
    avlTree->draw(window);

    window.setView(window.getDefaultView()); // Trả lại Camera góc cố định để vẽ UI

    btnBackToMenu->draw(window); inputVal->draw(window); 
    btnInsert->draw(window); btnSearch->draw(window); btnDelete->draw(window); btnInit->draw(window);
    btnStepBack->draw(window); btnPausePlay->draw(window); btnStepForward->draw(window);
    btnSpeedDown->draw(window); btnSpeedUp->draw(window);
}