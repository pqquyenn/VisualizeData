#include "AVLScreen.h"
#include "App.h"
#include "MainMenu.h"

AVLScreen::AVLScreen(App* app) : State(app) {
    // Khởi tạo các nút UI (Giống SLLScreen)
    btnBackToMenu = new Button(20, 20, 120, 40, app->font, "Back Menu");
    inputVal = new InputBox(150, 80, 100, 40, app->font, 3);
    
    btnInsert = new Button(270, 80, 100, 40, app->font, "Insert");
    btnSearch = new Button(390, 80, 100, 40, app->font, "Search");
    btnDelete = new Button(510, 80, 100, 40, app->font, "Delete");
    btnInit = new Button(630, 80, 120, 40, app->font, "Init Random");

    // Khởi tạo AVL Tree
    avlTree = new AVLTree(app->font);
    
    // Playback (Giả sử bạn đã có PlaybackController như hình bạn gửi)
    playbackController = new PlaybackController(250, 140, app->font, nullptr); 
    // Lưu ý: Cần sửa PlaybackController để nhận AVLTree thay vì SinglyLinkedList
}

AVLScreen::~AVLScreen() {
    // Xóa bộ nhớ...
    delete btnBackToMenu; delete btnInsert; delete btnSearch; 
    delete btnDelete; delete btnInit; delete inputVal;
    delete avlTree; delete playbackController;
}

void AVLScreen::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    // --- CAMERA LOGIC (PAN & ZOOM) ---
    if (!isViewInitialized) {
        treeView = window.getDefaultView();
        isViewInitialized = true;
    }
    if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
        if (event.mouseWheelScroll.delta > 0) treeView.zoom(0.9f);
        else if (event.mouseWheelScroll.delta < 0) treeView.zoom(1.1f);
    }
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        if (pixelPos.y > 200) { // Click dưới vùng UI mới cho kéo
            isPanning = true;
            oldMousePos = pixelPos;
        }
    }
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        isPanning = false;
    }
    if (event.type == sf::Event::MouseMoved && isPanning) {
        sf::Vector2i newMousePos = sf::Mouse::getPosition(window);
        sf::Vector2f delta = window.mapPixelToCoords(oldMousePos, treeView) - window.mapPixelToCoords(newMousePos, treeView);
        treeView.move(delta);
        oldMousePos = newMousePos;
    }

    // --- UI LOGIC ---
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    inputVal->handleEvent(event, mousePos);

    if (btnBackToMenu->isClicked(event, mousePos)) {
        app->changeState(new MainMenu(app));
        return;
    }
    
    if (btnInsert->isClicked(event, mousePos)) {
        std::string valStr = inputVal->getText();
        if (!valStr.empty()) {
            int val = std::stoi(valStr);
            avlTree->insert(val); // Gọi Insert của AVL
            inputVal->clear();
        }
    }
    
    // TODO: Thêm logic click cho Delete, Search, Init...
}

void AVLScreen::update(float deltaTime, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    // Hiệu ứng hover cho nút
    btnBackToMenu->update(mousePos);
    btnInsert->update(mousePos);
    btnSearch->update(mousePos);
    btnDelete->update(mousePos);
    btnInit->update(mousePos);
    
    // Cập nhật animation di chuyển của cây
    avlTree->update(deltaTime);
}

void AVLScreen::draw(sf::RenderWindow& window) {
    // 1. Áp dụng Camera để vẽ cây AVL
    if (isViewInitialized) {
        window.setView(treeView);
    }
    avlTree->draw(window);

    // 2. Trả về Default View để vẽ UI cố định
    window.setView(window.getDefaultView());
    
    btnBackToMenu->draw(window);
    inputVal->draw(window);
    btnInsert->draw(window);
    btnSearch->draw(window);
    btnDelete->draw(window);
    btnInit->draw(window);
    
    // playbackController->draw(window);
}