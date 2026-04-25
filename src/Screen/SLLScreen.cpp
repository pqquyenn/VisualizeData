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
    inputVal = new InputBox(150, 80, 100, 40, app->font, 3);
    
    // Đổi tên nhãn Insert thành Ins Tail cho rõ ràng (tuỳ chọn)
    btnInsert = new Button(270, 80, 100, 40, app->font, "Ins Tail");
    btnSearch = new Button(390, 80, 100, 40, app->font, "Search");
    btnDelete = new Button(510, 80, 100, 40, app->font, "Delete");
    btnInit = new Button(630, 80, 100, 40, app->font, "Init");
    btnInitFile = new Button(750, 80, 100, 40, app->font, "Init File");

    // --- KHỞI TẠO UI MỚI (Hàng 2: y = 140) ---
    btnInsertHead = new Button(150, 140, 100, 40, app->font, "Ins Head");
    btnInsertIndex = new Button(270, 140, 100, 40, app->font, "Ins Index");
    
    // Ô inputIndex và nút Go (ẩn mặc định, sẽ được đặt cạnh Ins Index)
    inputIndex = new InputBox(390, 140, 60, 40, app->font, 2); 
    btnGoInsertIndex = new Button(470, 140, 50, 40, app->font, "Go");

    // --- DỜI UI PLAYBACK XUỐNG DƯỚI (Hàng 3: y = 200) ---
// --- DỜI UI PLAYBACK XUỐNG DƯỚI (Hàng 3: y = 200) ---
    // Cập nhật lại toạ độ X để nhét thêm 2 nút mới cho cân xứng
    btnSpeedDown   = new Button(130, 200, 50, 40, app->font, "<<");
    btnSkipBack    = new Button(190, 200, 40, 40, app->font, "|<"); // Nút mới
    btnStepBack    = new Button(240, 200, 40, 40, app->font, "<");
    btnPausePlay   = new Button(290, 200, 120, 40, app->font, "Pause/Play");
    btnStepForward = new Button(420, 200, 40, 40, app->font, ">");
    btnSkipForward = new Button(470, 200, 40, 40, app->font, ">|"); // Nút mới
    btnSpeedUp     = new Button(520, 200, 50, 40, app->font, ">>");

    // Tìm đến phần khởi tạo các nút hàng 3 (y = 200), ngay dưới dòng khởi tạo btnSpeedUp:
    btnSpeedUp     = new Button(520, 200, 50, 40, app->font, ">>");

    // THÊM ĐOẠN NÀY VÀO ĐỂ KHỞI TẠO TEXT TỐC ĐỘ:
    textSpeed.setFont(app->font);
    textSpeed.setCharacterSize(16);
    textSpeed.setFillColor(sf::Color::White);
    textSpeed.setPosition(590, 210); // Đặt bên phải nút ">>"

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
    
    inputVal->handleEvent(event, mousePos); 
    if (showIndexInput) inputIndex->handleEvent(event, mousePos); // Chỉ gõ được index khi nó hiện

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

    // Nút Bật/Tắt ô nhập Index
    if (btnInsertIndex->isClicked(event, mousePos)) {
        showIndexInput = !showIndexInput; 
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

    // THÊM ĐOẠN NÀY TRƯỚC KHI GỌI sll->updateAnimation:
    // Tính toán tốc độ: delay mặc định là 0.8f tương đương 1.0x
    float currentDelay = sll->getDelay();
    float speedMultiplier = 0.8f / currentDelay;
    
    std::stringstream ss;
    ss << "Speed: " << std::fixed << std::setprecision(1) << speedMultiplier << "x";
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
    // THÊM VÀO DÒNG NÀY Ở CUỐI CÙNG:
    window.draw(textSpeed);
}