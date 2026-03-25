#include "HeapScreen.h"
#include "App.h"
#include "MainMenu.h"
#include <iostream>
#include <exception>

// --- FILE: src/Heap/HeapScreen.cpp ---

HeapScreen::HeapScreen(App* app) : State(app), isPanning(false), isViewInitialized(false) {
    // --- SỬA: Load font hệ thống chuẩn của Windows để không bao giờ bị lỗi ---
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        // Fallback: Thử đường dẫn phổ biến khác nếu không thấy
        if (!font.loadFromFile("external/ARIAL.TTF")) {
             std::cerr << "Khong the load font arial!\n";
        }
    }

    heapTree = new HeapTree(font);

    // --- SỬA: Căn lề UI hoàn hảo và đồng bộ X với dải AVL của bạn ---
    btnBackToMenu = new Button(20, 20, 100, 40, app->font, "Back Menu");
    inputVal = new InputBox(150, 80, 100, 40, app->font, 3);
    
    btnInsert = new Button(270, 80, 100, 40, app->font, "Insert");
    btnDeleteMax = new Button(390, 80, 100, 40, app->font, "Delete");
    btnInit = new Button(510, 80, 100, 40, app->font, "Init");

    // --- SỬA: Sắp xếp lại dải nút Playback chuẩn và đồng bộ X với dải AVL của bạn ---
    // Khối nút trải dài từ X = 250 đến 630, cách nhau 15 pixel
    btnSpeedDown   = new Button(250, 140, 50, 40, app->font, "<<");
    btnStepBack    = new Button(315, 140, 50, 40, app->font, "<");
    btnPausePlay   = new Button(380, 140, 120, 40, app->font, "Pause/Play");
    btnStepForward = new Button(515, 140, 50, 40, app->font, ">");
    btnSpeedUp     = new Button(580, 140, 50, 40, app->font, ">>");

    // Set Views
    treeView.setSize(1280, 720); // TreeView để pan/zoom/drag
    uiView.setSize(1280, 720);   // UIView cố định cho UI
    uiView.setCenter(640, 360);
}

HeapScreen::~HeapScreen() {
    delete heapTree; delete btnBackToMenu; delete btnInsert;
    delete btnDeleteMax; delete btnInit; delete inputVal;
    delete btnSpeedDown; delete btnStepBack; delete btnPausePlay;
    delete btnStepForward; delete btnSpeedUp;
}

void HeapScreen::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (!isViewInitialized) {
        treeView = window.getDefaultView();
        isViewInitialized = true;
    }

    // --- SỬA LỖI: Sắp xếp lại thứ tự ưu tiên sự kiện cho Panning và Dragging ---

    // Chuyển UIView để kiểm tra click UI và InputBox (Cần cố định)
    window.setView(uiView);
    sf::Vector2f mousePosUI = window.mapPixelToCoords(sf::Mouse::getPosition(window), uiView);
    inputVal->handleEvent(event, mousePosUI); 

    // Chuyển về TreeView để xử lý Kéo/Pan (Bị ảnh hưởng bởi Camera)
    window.setView(treeView);
    
    // --- 1. GỬI SỰ KIỆN KÉO THẢ CHO HEAPTREE (Logic từ AVL) ---
    // heapTree sẽ tự kiểm tra xem chuột có trúng node nào không.
    heapTree->handleEvent(event, window, treeView);

    // --- 2. XỬ LÝ PAN VÀ ZOOM CAMERA (Logic "Kéo thả lai") ---
    // Ta chỉ Pan khung hình nếu người dùng KHÔNG đang kéo thả một node nào cả.
    bool nodeIsBeingDragged = false;
    // (Cần bổ sung hàm getter trong HeapTree nếu muốn kiểm tra chính xác, 
    //  hoặc tạm thời chấp nhận panning khi drag node ở biên)
    
    if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
        if (event.mouseWheelScroll.delta > 0) treeView.zoom(0.9f); // Zoom in
        else treeView.zoom(1.1f); // Zoom out
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        // Tránh Pan khi đang click InputBox hoặc Buttons
        // (Kiểm tra xem chuột có nằm ngoài khu vực UI không)
        if (pixelPos.y > 200 && !nodeIsBeingDragged) { 
            isPanning = true; 
            oldMousePos = pixelPos; 
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        isPanning = false;
    }
    else if (event.type == sf::Event::MouseMoved && isPanning) {
        sf::Vector2f delta = window.mapPixelToCoords(oldMousePos, treeView) - window.mapPixelToCoords(sf::Mouse::getPosition(window), treeView);
        treeView.move(delta);
        oldMousePos = sf::Mouse::getPosition(window);
    }

    // --- 3. XỬ LÝ CLICK NÚT BẤM UI (Sử dụng mousePosUI và view mặc định/UIView) ---
    window.setView(uiView); // Chuyển về UI view để check click nút
    if (btnBackToMenu->isClicked(event, mousePosUI)) {
        app->changeState(new MainMenu(app)); 
        return;
    }

    // Playback logic
    if (btnPausePlay->isClicked(event, mousePosUI)) heapTree->togglePause();
    if (btnStepBack->isClicked(event, mousePosUI)) heapTree->stepBackward();
    if (btnStepForward->isClicked(event, mousePosUI)) heapTree->stepForward();
    if (btnSpeedDown->isClicked(event, mousePosUI)) heapTree->decreaseSpeed();
    if (btnSpeedUp->isClicked(event, mousePosUI)) heapTree->increaseSpeed();

    // Max Heap Logic (Insert, Delete, Init)
    if (btnInsert->isClicked(event, mousePosUI) || 
        btnDeleteMax->isClicked(event, mousePosUI) || btnInit->isClicked(event, mousePosUI)) {
        
        std::string txt = inputVal->getText();
        if (!txt.empty() || btnDeleteMax->isClicked(event, mousePosUI)) { // Xóa gốc ko cần input
            try {
                if (btnInsert->isClicked(event, mousePosUI)) {
                    heapTree->insertVal(std::stoi(txt));
                    inputVal->clear();
                }
                else if (btnDeleteMax->isClicked(event, mousePosUI)) {
                    heapTree->deleteMax();
                }
                else if (btnInit->isClicked(event, mousePosUI)) {
                    // --- SỬA: Init theo N linh hoạt lấy từ InputBox ---
                    heapTree->initTree(std::stoi(txt));
                    inputVal->clear();
                }
            } catch (const std::exception& e) {
                inputVal->clear();
            } 
        }
    }
}

void HeapScreen::update(float deltaTime, sf::RenderWindow& window) {
    // --- 1. LẤY TỌA ĐỘ CHUỘT UI CHO CẬP NHẬT HOVER ---
    sf::Vector2f mousePosUI = window.mapPixelToCoords(sf::Mouse::getPosition(window), uiView);
    btnBackToMenu->update(mousePosUI);
    btnInsert->update(mousePosUI); btnDeleteMax->update(mousePosUI); btnInit->update(mousePosUI);
    btnSpeedDown->update(mousePosUI); btnStepBack->update(mousePosUI); btnPausePlay->update(mousePosUI);
    btnStepForward->update(mousePosUI); btnSpeedUp->update(mousePosUI);

    // --- 2. CẬP NHẬT HOẠT ẢNH CHO CÂY ---
    heapTree->updateAnimation(deltaTime);
    heapTree->updatePosition(deltaTime); // Cập nhật vị trí mượt của Node
}

void HeapScreen::draw(sf::RenderWindow& window) {
    window.clear(sf::Color(40, 45, 52)); // Màu nền tối đồng bộ

    // --- 1. Vẽ Cây với Tree View (Bị ảnh hưởng bởi Pan/Zoom/Drag) ---
    if (isViewInitialized) window.setView(treeView);
    heapTree->draw(window);

    // --- 2. Trả lại Camera góc cố định để vẽ UI ---
    window.setView(uiView); 
    btnBackToMenu->draw(window); inputVal->draw(window); 
    btnInsert->draw(window); btnDeleteMax->draw(window); btnInit->draw(window);
    btnSpeedDown->draw(window); btnStepBack->draw(window); btnPausePlay->draw(window);
    btnStepForward->draw(window); btnSpeedUp->draw(window);
}