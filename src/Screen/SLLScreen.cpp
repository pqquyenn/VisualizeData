#include "SLLScreen.h"
#include "App.h"
#include "MainMenu.h"
#include <string>
#include <exception>

// THÊM 2 THƯ VIỆN NÀY ĐỂ FORMAT SỐ THẬP PHÂN
#include <sstream>
#include <iomanip>

SLLScreen::SLLScreen(App* app) : State(app) {
    btnBackToMenu = new Button(20, 20, 110, 40, app->font, "Back Menu");
    
    // --- HÀNG 1 (y = 80): Các thao tác chính liền mạch ---
    inputVal = new InputBox(150, 80, 100, 40, app->font, 3);
    btnSearch = new Button(270, 80, 100, 40, app->font, "Search");
    btnDelete = new Button(390, 80, 100, 40, app->font, "Delete");
    btnInit = new Button(510, 80, 100, 40, app->font, "Init");
    btnInitFile = new Button(630, 80, 100, 40, app->font, "Init File");
    btnUpdate = new Button(750, 80, 100, 40, app->font, "Update");
    btnInsertHead = new Button(870, 80, 100, 40, app->font, "Ins Head");
    btnInsert = new Button(990, 80, 100, 40, app->font, "Ins Tail");
    btnInsertIndex = new Button(1110, 80, 100, 40, app->font, "Ins Index");

    // --- HÀNG 2 (y = 140): Playback Controls & Popups ---
    
    // 1. Cụm Playback (Đẩy lên y = 140, nằm bên trái để lấp khoảng trống)
    btnSpeedDown   = new Button(150, 140, 50, 40, app->font, "<<");
    btnSkipBack    = new Button(210, 140, 40, 40, app->font, "|<"); 
    btnStepBack    = new Button(260, 140, 40, 40, app->font, "<");
    btnPausePlay   = new Button(310, 140, 120, 40, app->font, "Pause/Play");
    btnStepForward = new Button(440, 140, 40, 40, app->font, ">");
    btnSkipForward = new Button(490, 140, 40, 40, app->font, ">|"); 
    btnSpeedUp     = new Button(540, 140, 50, 40, app->font, ">>");

    // Chỉnh lại vị trí chữ hiển thị Tốc Độ cho thẳng hàng 2
    textSpeed.setFont(app->font);
    textSpeed.setCharacterSize(16);
    textSpeed.setFillColor(sf::Color::White);
    textSpeed.setPosition(600, 150); 

    // 2. Cụm Popup (Nằm ngay dưới nút tương ứng ở hàng 1)
    // Popup Update (Dưới nút Update x = 750)
    inputUpdateIndex = new InputBox(750, 140, 60, 40, app->font, 2); 
    btnGoUpdate = new Button(820, 140, 50, 40, app->font, "Go");

    // Popup Ins Index (Dưới nút Ins Index x = 1110)
    inputIndex = new InputBox(1110, 140, 60, 40, app->font, 2); 
    btnGoInsertIndex = new Button(1180, 140, 50, 40, app->font, "Go");

    sll = new SinglyLinkedList(app->font);
}

SLLScreen::~SLLScreen() {
    delete btnBackToMenu; delete btnInsert; delete btnSearch; 
    delete btnDelete; delete btnInit; delete btnInitFile; delete inputVal;
    
    // Xoá pointer mới
    delete btnInsertHead; delete btnInsertIndex; 
    delete inputIndex; delete btnGoInsertIndex;

    delete btnStepBack; delete btnPausePlay; delete btnStepForward;
    delete btnSpeedDown; delete btnSpeedUp;
    delete sll;

    // Thêm 2 nút này vào phần delete
    delete btnSkipBack; 
    delete btnSkipForward;
    delete btnUpdate; delete inputUpdateIndex; delete btnGoUpdate;
}

void SLLScreen::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (!isViewInitialized) {
        listView = window.getDefaultView();
        isViewInitialized = true;
    }

    if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
        if (event.mouseWheelScroll.delta > 0) listView.zoom(0.9f);
        else if (event.mouseWheelScroll.delta < 0) listView.zoom(1.1f);
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        // Lưu ý: Đổi điều kiện y > 260 để không bị vướng lúc bấm các nút Playback (y=200)
        if (pixelPos.y > 260) { isPanning = true; oldMousePos = pixelPos; }
    }
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) isPanning = false;
    
    if (event.type == sf::Event::MouseMoved && isPanning) {
        sf::Vector2i newMousePos = sf::Mouse::getPosition(window);
        sf::Vector2f delta = window.mapPixelToCoords(oldMousePos, listView) - window.mapPixelToCoords(newMousePos, listView);
        listView.move(delta);
        oldMousePos = newMousePos;
    }

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    if (btnBackToMenu->isClicked(event, mousePos)) {
        app->changeState(new MainMenu(app)); 
        return; 
    }
    


    if (btnPausePlay->isClicked(event, mousePos)) sll->togglePause();
    
    if (btnSkipBack->isClicked(event, mousePos)) sll->skipToFirstStep();       // THÊM MỚI
    if (btnStepBack->isClicked(event, mousePos)) sll->stepBackward();
    if (btnStepForward->isClicked(event, mousePos)) sll->stepForward();
    if (btnSkipForward->isClicked(event, mousePos)) sll->skipToLastStep();
    if (btnSpeedDown->isClicked(event, mousePos)) sll->decreaseSpeed();
    if (btnSpeedUp->isClicked(event, mousePos)) sll->increaseSpeed();

    if (btnInitFile->isClicked(event, mousePos)) {
        sll->initFromFile("input.txt"); 
        inputVal->clear();
    }

    inputVal->handleEvent(event, mousePos); 
    if (showIndexInput) inputIndex->handleEvent(event, mousePos); // Chỉ gõ được index khi nó hiện
    if (showUpdateInput) inputUpdateIndex->handleEvent(event, mousePos);

    // Nút Bật/Tắt ô nhập Index
    if (btnInsertIndex->isClicked(event, mousePos)) {
        showIndexInput = !showIndexInput; 
        showUpdateInput = false; // Tắt ô update nếu đang mở
    }

    // Xử lý bật/tắt ô nhập Update
    if (btnUpdate->isClicked(event, mousePos)) {
        showUpdateInput = !showUpdateInput;
        showIndexInput = false; // Tắt ô insert index nếu đang mở
    }

    // Xử lý Update khi bấm nút Go
    if (showUpdateInput && btnGoUpdate->isClicked(event, mousePos)) {
        std::string valText = inputVal->getText();
        std::string idxText = inputUpdateIndex->getText();
        if (!valText.empty() && !idxText.empty()) {
            try {
                int value = std::stoi(valText);
                int index = std::stoi(idxText);
                sll->updateNode(value, index);
                inputVal->clear();
                inputUpdateIndex->clear();
                showUpdateInput = false; // Ẩn UI sau khi chạy
            } catch (const std::exception& e) {
                inputVal->clear(); inputUpdateIndex->clear();
            }
        }
    }

    // Xử lý Insert Index khi bấm nút Go
    if (showIndexInput && btnGoInsertIndex->isClicked(event, mousePos)) {
        std::string valText = inputVal->getText();
        std::string idxText = inputIndex->getText();
        if (!valText.empty() && !idxText.empty()) {
            try {
                int value = std::stoi(valText);
                int index = std::stoi(idxText);
                sll->insertAtIndex(value, index);
                inputVal->clear();
                inputIndex->clear();
                showIndexInput = false; // Tắt UI sau khi chạy xong
            } catch (const std::exception& e) {
                inputVal->clear(); inputIndex->clear();
            }
        }
    }

    // Xử lý các thao tác khác
    if (btnInsert->isClicked(event, mousePos) || btnInsertHead->isClicked(event, mousePos) || 
        btnSearch->isClicked(event, mousePos) || btnDelete->isClicked(event, mousePos) || btnInit->isClicked(event, mousePos)) {
        
        std::string text = inputVal->getText();
        if (!text.empty()) {
            try {
                int value = std::stoi(text); 
                if (btnInsert->isClicked(event, mousePos)) sll->insertNode(value); // Insert Tail
                else if (btnInsertHead->isClicked(event, mousePos)) sll->insertAtHead(value);
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
    
    btnBackToMenu->update(mousePos); btnInsert->update(mousePos);
    btnSearch->update(mousePos); btnDelete->update(mousePos); 
    btnInit->update(mousePos); btnInitFile->update(mousePos);
    
    // Update nút mới
    btnInsertHead->update(mousePos);
    btnInsertIndex->update(mousePos);
    if (showIndexInput) {
        btnGoInsertIndex->update(mousePos);
    }

    btnStepBack->update(mousePos); btnPausePlay->update(mousePos); btnStepForward->update(mousePos);
    btnSpeedDown->update(mousePos); btnSpeedUp->update(mousePos);
    btnUpdate->update(mousePos);
    if (showUpdateInput) {
        // inputUpdateIndex->update(mousePos);
        btnGoUpdate->update(mousePos);
    }

    // THÊM ĐOẠN NÀY TRƯỚC KHI GỌI sll->updateAnimation:
    // Tính toán tốc độ: delay mặc định là 0.8f tương đương 1.0x
// Xoá đoạn code tính toán speedMultiplier cũ và THAY BẰNG ĐOẠN SAU:
    float currentSpeed = sll->getSpeedMultiplier();
    
    std::stringstream ss;
    // Set precision(2) để hiển thị đẹp các số .25 và .75
    ss << "Speed: " << std::fixed << std::setprecision(2) << currentSpeed << "x";
    textSpeed.setString(ss.str());

    // Thêm update cho 2 nút mới
    btnSkipBack->update(mousePos); 
    btnSkipForward->update(mousePos);

    sll->updateAnimation(deltaTime);
    sll->updatePosition(deltaTime);
}

void SLLScreen::draw(sf::RenderWindow& window) {
    if (isViewInitialized) window.setView(listView);
    sll->draw(window);

    // Bắt buộc reset View về Default để UI & CodeBox không bị trôi
    window.setView(window.getDefaultView()); 
    sll->drawCode(window); 

    btnBackToMenu->draw(window); inputVal->draw(window); 
    btnInsert->draw(window); btnSearch->draw(window); btnDelete->draw(window); 
    btnInit->draw(window); btnInitFile->draw(window);
    
    // Vẽ UI mới
    btnInsertHead->draw(window);
    btnInsertIndex->draw(window);
    if (showIndexInput) {
        inputIndex->draw(window);
        btnGoInsertIndex->draw(window);
    }
    // Thêm draw cho 2 nút mới
    btnSkipBack->draw(window); 
    btnSkipForward->draw(window);
    btnStepBack->draw(window); btnPausePlay->draw(window); btnStepForward->draw(window);
    btnSpeedDown->draw(window); btnSpeedUp->draw(window);
    // Trong hàm draw(...):
    btnUpdate->draw(window);
    if (showUpdateInput) {
        inputUpdateIndex->draw(window);
        btnGoUpdate->draw(window);
    }
    // THÊM VÀO DÒNG NÀY Ở CUỐI CÙNG:
    window.draw(textSpeed);
}