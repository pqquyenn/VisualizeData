#include "SLLScreen.h"
#include "App.h"
#include "MainMenu.h"
#include <string>
#include <exception>

SLLScreen::SLLScreen(App* app) : State(app) {
    // Nút Back ở góc trái trên
    btnBack = new Button(20, 20, 100, 40, app->font, "Back");
    
    // Ô nhập liệu (InputBox)
    inputVal = new InputBox(150, 80, 100, 40, app->font, 3);
    
    // Các nút chức năng đặt dàn hàng ngang cạnh InputBox
    btnInsert = new Button(270, 80, 100, 40, app->font, "Insert");
    btnSearch = new Button(390, 80, 100, 40, app->font, "Search");
    btnDelete = new Button(510, 80, 100, 40, app->font, "Delete");
    
    // Khởi tạo danh sách liên kết
    sll = new SinglyLinkedList(app->font);
}

SLLScreen::~SLLScreen() {
    delete btnBack;
    delete btnInsert;
    delete btnSearch;
    delete btnDelete;
    delete inputVal; 
    delete sll;
}

void SLLScreen::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    if (btnBack->isClicked(event, mousePos)) {
        app->changeState(new MainMenu(app)); 
        return; // Đổi state xong thì thoát luôn hàm để tránh lỗi
    }
    
    inputVal->handleEvent(event, mousePos); 

    // Xử lý khi người dùng click vào 1 trong 3 nút chức năng
    if (btnInsert->isClicked(event, mousePos) || 
        btnSearch->isClicked(event, mousePos) || 
        btnDelete->isClicked(event, mousePos)) {
        
        std::string text = inputVal->getText();
        if (!text.empty()) {
            try {
                int value = std::stoi(text); 
                
                // Kiểm tra xem nút nào được bấm
                if (btnInsert->isClicked(event, mousePos)) {
                    sll->insertNode(value);
                } 
                else if (btnSearch->isClicked(event, mousePos)) {
                    sll->startSearch(value);
                } 
                else if (btnDelete->isClicked(event, mousePos)) {
                    sll->deleteNode(value);
                }

                inputVal->clear(); // Xóa ô nhập sau khi thực hiện
            } catch (const std::exception& e) {
                inputVal->clear(); // Nếu nhập linh tinh không phải số thì xóa trắng
            }        
        }
    }
}

void SLLScreen::update(float deltaTime, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    btnBack->update(mousePos);
    btnInsert->update(mousePos);
    btnSearch->update(mousePos);
    btnDelete->update(mousePos);

    // Cập nhật hoạt ảnh của danh sách liên kết
    sll->updatePosition(deltaTime);
    sll->updateSearching(deltaTime);
}

void SLLScreen::draw(sf::RenderWindow& window) {
    btnBack->draw(window);
    inputVal->draw(window); 
    btnInsert->draw(window);
    btnSearch->draw(window);
    btnDelete->draw(window);
    
    // Vẽ danh sách liên kết
    sll->draw(window);
}