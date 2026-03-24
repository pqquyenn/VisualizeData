#include "MainMenu.h"
#include "App.h"
#include "SLLScreen.h"
#include "AVLScreen.h"

MainMenu::MainMenu(App* app) : State(app) {
    // Tạo nút bấm đi tới Singly Linked List (app->font là lấy font từ class App)
    btnSLL = new Button(540, 300, 200, 50, app->font, "Singly Linked List");
    // SUA CODE O DAY
    btnAVL = new Button(540, 400, 200, 50, app->font, "AVL tree");
}

MainMenu::~MainMenu() {
    delete btnSLL;
    delete btnAVL;
}

void MainMenu::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    // Nếu click vào nút SLL, bảo App chuyển sang màn hình SLLScreen
    if (btnSLL->isClicked(event, mousePos)) {
        app->changeState(new SLLScreen(app)); 
    }
    if (btnAVL->isClicked(event, mousePos)) {
        app->changeState(new AVLScreen(app)); 
    }
}

void MainMenu::update(float deltaTime, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    btnSLL->update(mousePos); // Để nút đổi màu khi hover
    btnAVL->update(mousePos);
}

void MainMenu::draw(sf::RenderWindow& window) {
    btnSLL->draw(window);
    btnAVL->draw(window);
}