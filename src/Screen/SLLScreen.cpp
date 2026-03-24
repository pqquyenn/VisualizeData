#include "SLLScreen.h"
#include "App.h"
#include "MainMenu.h"
#include <string>
#include <exception>

SLLScreen::SLLScreen(App* app) : State(app) {
    btnBackToMenu = new Button(20, 20, 100, 40, app->font, "Back Menu");
    inputVal = new InputBox(150, 80, 100, 40, app->font, 3);
    
    btnInsert = new Button(270, 80, 100, 40, app->font, "Insert");
    btnSearch = new Button(390, 80, 100, 40, app->font, "Search");
    btnDelete = new Button(510, 80, 100, 40, app->font, "Delete");
    btnInit = new Button(630, 80, 100, 40, app->font, "Init Random");

    sll = new SinglyLinkedList(app->font);
    
    // Khởi tạo cụm nút playback và truyền sll vào
    playbackController = new PlaybackController(200, 140, app->font, sll);
}

SLLScreen::~SLLScreen() {
    delete btnBackToMenu;
    delete btnInsert;
    delete btnSearch;
    delete btnDelete;
    delete btnInit;
    delete inputVal; 
    delete sll;
    delete playbackController;
}

void SLLScreen::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    if (btnBackToMenu->isClicked(event, mousePos)) {
        app->changeState(new MainMenu(app)); 
        return;
    }
    
    inputVal->handleEvent(event, mousePos); 
    playbackController->handleEvent(event, mousePos);

    if (btnInsert->isClicked(event, mousePos) || 
        btnSearch->isClicked(event, mousePos) || 
        btnDelete->isClicked(event, mousePos) ||
        btnInit->isClicked(event, mousePos)) {
        
        std::string text = inputVal->getText();
        if (!text.empty()) {
            try {
                int value = std::stoi(text); 
                
                if (btnInsert->isClicked(event, mousePos)) sll->insertNode(value);
                else if (btnSearch->isClicked(event, mousePos)) sll->startSearch(value);
                else if (btnDelete->isClicked(event, mousePos)) sll->startDelete(value); 
                else if (btnInit->isClicked(event, mousePos)) sll->initList(value); 

                inputVal->clear();
            } catch (const std::exception& e) {
                inputVal->clear();
            }        
        }
    }
}

void SLLScreen::update(float deltaTime, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    btnBackToMenu->update(mousePos);
    btnInsert->update(mousePos);
    btnSearch->update(mousePos);
    btnDelete->update(mousePos);
    btnInit->update(mousePos);

    playbackController->update(mousePos);

    sll->updatePosition(deltaTime);
    sll->updateAnimation(deltaTime);
}

void SLLScreen::draw(sf::RenderWindow& window) {
    btnBackToMenu->draw(window);
    inputVal->draw(window); 
    btnInsert->draw(window);
    btnSearch->draw(window);
    btnDelete->draw(window);
    btnInit->draw(window);
    
    playbackController->draw(window);
    sll->draw(window);
}