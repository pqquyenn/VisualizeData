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
    btnInit = new Button(630, 80, 100, 40, app->font, "Init");


    // THÊM KHỞI TẠO NÚT TỐC ĐỘ (Toạ độ nối tiếp sau nút Init)
    btnSpeedDown = new Button(750, 80, 50, 40, app->font, "<<");
    btnSpeedUp = new Button(810, 80, 50, 40, app->font, ">>");
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

    delete btnInit;
    delete btnSpeedDown; // THÊM
    delete btnSpeedUp;   // THÊM
}

void SLLScreen::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    // 1. Khởi tạo Camera bằng kích thước cửa sổ ở khung hình đầu tiên
    if (!isViewInitialized) {
        listView = window.getDefaultView();
        isViewInitialized = true;
    }

    // 2. Xử lý Zoom (Lăn chuột)
    if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
        if (event.mouseWheelScroll.delta > 0) {
            listView.zoom(0.9f); // Lăn lên: Phóng to
        } else if (event.mouseWheelScroll.delta < 0) {
            listView.zoom(1.1f); // Lăn xuống: Thu nhỏ
        }
    }

    // 3. Xử lý Kéo/Pan (Bấm giữ chuột trái và di chuyển)
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        // Chỉ cho phép kéo thả nếu bấm ở khu vực trống bên dưới các nút UI (y > 200)
        // để tránh việc vừa bấm nút vừa vô tình kéo màn hình.
        if (pixelPos.y > 200) {
            isPanning = true;
            oldMousePos = pixelPos;
        }
    }
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        isPanning = false; // Thả chuột ra thì ngừng kéo
    }
    if (event.type == sf::Event::MouseMoved && isPanning) {
        sf::Vector2i newMousePos = sf::Mouse::getPosition(window);
        // Tính khoảng cách chuột đã di chuyển và chuyển đổi sang toạ độ Camera
        sf::Vector2f delta = window.mapPixelToCoords(oldMousePos, listView) - window.mapPixelToCoords(newMousePos, listView);
        listView.move(delta); // Dịch chuyển Camera
        oldMousePos = newMousePos; // Cập nhật lại vị trí chuột
    }
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    if (btnBackToMenu->isClicked(event, mousePos)) {
        app->changeState(new MainMenu(app)); 
        return;
    }
    // 3. Trong SLLScreen::handleEvent
    // ... (sau đoạn if btnBackToMenu)
    if (btnSpeedDown->isClicked(event, mousePos)) sll->decreaseSpeed();
    if (btnSpeedUp->isClicked(event, mousePos)) sll->increaseSpeed();
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
// 4. Trong SLLScreen::update
    btnInit->update(mousePos);
    btnSpeedDown->update(mousePos); // THÊM
    btnSpeedUp->update(mousePos);   // THÊM

    playbackController->update(mousePos);

    sll->updatePosition(deltaTime);
    sll->updateAnimation(deltaTime);
}

void SLLScreen::draw(sf::RenderWindow& window) {
    // === BƯỚC 1: LẮP CAMERA VÀO ĐỂ VẼ DANH SÁCH ===
    if (isViewInitialized) {
        window.setView(listView);
    }
    
    // Chỉ vẽ SLL ở chế độ Camera này
    sll->draw(window);

    // === BƯỚC 2: GỠ CAMERA RA, TRẢ VỀ CỐ ĐỊNH ĐỂ VẼ GIAO DIỆN (UI) ===
    window.setView(window.getDefaultView());

    // Vẽ toàn bộ các nút bấm và ô nhập liệu (sẽ dính chặt vào màn hình)
    btnBackToMenu->draw(window);
    inputVal->draw(window); 
    btnInsert->draw(window);
    btnSearch->draw(window);
    btnDelete->draw(window);
// 5. Trong SLLScreen::draw (Ở BƯỚC 2: GỠ CAMERA RA)
    btnInit->draw(window);
    btnSpeedDown->draw(window); // THÊM
    btnSpeedUp->draw(window);   // THÊM
    
    playbackController->draw(window);
}