#include "SLLScreen.h"
#include "App.h"
#include "MainMenu.h"

SLLScreen::SLLScreen(App* app) : State(app) {
    btnBack = new Button(20, 20, 100, 40, app->font, "Back");
    btnInsert = new Button(20, 80, 100, 40, app->font, "Search"); // Tạm đổi tên thành Search vì bạn viết hàm startSearch rồi
    
    inputVal = new InputBox(150, 80, 100, 40, app->font, 3);
    
    // Khởi tạo danh sách liên kết
    sll = new SinglyLinkedList(app->font);
}

SLLScreen::~SLLScreen() {
    delete btnBack;
    delete btnInsert;
    delete inputVal; 
    delete sll; // Nhớ xóa sll
}

void SLLScreen::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    if (btnBack->isClicked(event, mousePos)) {
        app->changeState(new MainMenu(app)); 
    }
    
    inputVal->handleEvent(event, mousePos); 

if (btnInsert->isClicked(event, mousePos)) {
        std::string text = inputVal->getText();
        if (!text.empty()) {
            try {
                int value = std::stoi(text); 
                sll->startSearch(value);   
                inputVal->clear();  
            } catch (const std::exception& e) {
                // Lỡ có lỗi ép kiểu thì app vẫn sống, chỉ xóa ô input đi thôi
                inputVal->clear();
            }        
        }
    }
}

void SLLScreen::update(float deltaTime, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    btnBack->update(mousePos);
    btnInsert->update(mousePos);

    // Cập nhật hoạt ảnh của danh sách liên kết
    sll->updatePosition(deltaTime);
    sll->updateSearching(deltaTime);
}

void SLLScreen::draw(sf::RenderWindow& window) {
    btnBack->draw(window);
    btnInsert->draw(window);
    inputVal->draw(window); 
    
    // Vẽ danh sách liên kết
    sll->draw(window);
}