#include "AVLScreen.h"
#include "App.h"
#include "MainMenu.h"
#include <string>
#include <exception>

AVLScreen::AVLScreen(App* app) : State(app) {
    btnBackToMenu = new Button(20, 20, 110, 40, app->font, "Back Menu");
    inputVal = new InputBox(150, 80, 100, 40, app->font, 3);
    
    btnInsert = new Button(270, 80, 100, 40, app->font, "Insert");
    btnSearch = new Button(390, 80, 100, 40, app->font, "Search");
    btnDelete = new Button(510, 80, 100, 40, app->font, "Delete");
    btnInit = new Button(630, 80, 100, 40, app->font, "Init");
    btnInitFromFile = new Button(750, 80, 100, 40, app->font, "Init File");

    // --- Đã sửa: Sắp xếp lại thứ tự và căn lề hoàn hảo cho các nút Playback ---
    // Khối nút trải dài từ X = 250 đến 630, cách nhau 15 pixel
    // btnSpeedDown   = new Button(250, 140, 50, 40, app->font, "<<");
    // btnStepBack    = new Button(315, 140, 50, 40, app->font, "<");
    // btnPausePlay   = new Button(380, 140, 120, 40, app->font, "Pause/Play");
    // btnStepForward = new Button(515, 140, 50, 40, app->font, ">");
    // btnSpeedUp     = new Button(580, 140, 50, 40, app->font, ">>");


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
    delete btnInitFromFile; // Nhớ xóa vùng nhớ
}

void AVLScreen::handleEvent(sf::Event& event, sf::RenderWindow& window) {
if (!isViewInitialized) {
        treeView = window.getDefaultView();
        isViewInitialized = true;
    }

    // Zoom Camera
    if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
        if (event.mouseWheelScroll.delta > 0) treeView.zoom(0.9f);
        else if (event.mouseWheelScroll.delta < 0) treeView.zoom(1.1f);
    }

    // --- 1. CHUYỂN TIẾP SỰ KIỆN CHO CÂY XỬ LÝ TRƯỚC ---
    // Phải setView thành treeView để toạ độ chuột ánh xạ đúng vào cây
    window.setView(treeView);
    avlTree->handleEvent(event, window, treeView);

    // --- 2. XỬ LÝ KÉO CAMERA (PANNING) ---
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        
        // CHỈ pan camera khi:
        // 1. Click chuột dưới vùng UI (y > 200)
        // 2. KHÔNG có Node nào đang bị kéo (!avlTree->isDraggingNode())
        if (pixelPos.y > 200 && !avlTree->isDraggingNode()) { 
            isPanning = true; 
            oldMousePos = pixelPos; 
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        isPanning = false;
    }
    else if (event.type == sf::Event::MouseMoved && isPanning) {
        sf::Vector2i newMousePos = sf::Mouse::getPosition(window);
        sf::Vector2f delta = window.mapPixelToCoords(oldMousePos, treeView) - window.mapPixelToCoords(newMousePos, treeView);
        treeView.move(delta);
        oldMousePos = newMousePos;
    }

    // Trả lại View mặc định để xử lý click cho UI Buttons
    window.setView(window.getDefaultView());
    // avlTree->handleEvent(event, window, treeView);


    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    // Đọc File
    if (btnInitFromFile->isClicked(event, mousePos)) {
        std::ifstream file("input.txt");
        if (!file.is_open()) file.open("../input.txt"); // Tìm thư mục cha nếu đang chạy trong build/

        if (file.is_open()) {
            std::vector<int> data;
            int val;
            while (file >> val) data.push_back(val);
            file.close();
            if (!data.empty()) avlTree->initFromFile(data);
        }
    }
    
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
    btnInitFromFile->update(mousePos);
}

void AVLScreen::draw(sf::RenderWindow& window) {
if (isViewInitialized) window.setView(treeView);
    avlTree->draw(window);

    window.setView(window.getDefaultView()); // Trả lại Camera góc cố định để vẽ UI

    btnBackToMenu->draw(window); inputVal->draw(window); 
    btnInsert->draw(window); btnSearch->draw(window); btnDelete->draw(window); 
    btnInit->draw(window); btnInitFromFile->draw(window); // Vẽ Init File
    // ... Vẽ các nút Playback ...
    
    drawCodeBlock(window); // Vẽ khung pseudo-code
    btnStepBack->draw(window); btnPausePlay->draw(window); btnStepForward->draw(window);
    btnSpeedDown->draw(window); btnSpeedUp->draw(window);
}

// 5. THÊM HÀM VẼ KHUNG CODE XUỐNG DƯỚI CÙNG
void AVLScreen::drawCodeBlock(sf::RenderWindow& window) {
    std::string op = avlTree->getCurrentOperation();
    if (op != "Insert" && op != "Delete" && op != "Search") return;

    const std::vector<std::string>* codePtr = &codeInsert;
    if (op == "Delete") codePtr = &codeDelete;
    else if (op == "Search") codePtr = &codeSearch;

    int activeLine = avlTree->getCurrentActiveLine();

    // Thông số khung nền màu be (giống hệt Max Heap)
    float boxX = 850.0f;
    float boxY = 400.0f;
    float boxWidth = 380.0f;
    float boxHeight = codePtr->size() * 30.0f + 20.0f;

    sf::RectangleShape bg(sf::Vector2f(boxWidth, boxHeight));
    bg.setPosition(boxX, boxY);
    bg.setFillColor(sf::Color(253, 246, 227)); // Màu nền sáng
    bg.setOutlineThickness(2.0f);
    bg.setOutlineColor(sf::Color(200, 200, 200));
    window.draw(bg);

    // Vẽ từng dòng chữ
    for (size_t i = 0; i < codePtr->size(); ++i) {
        float lineY = boxY + 10.0f + i * 30.0f;

        // Nếu là dòng đang chạy (activeLine), vẽ dải màu vàng làm highlight
        if ((int)i == activeLine) {
            sf::RectangleShape highlight(sf::Vector2f(boxWidth, 30.0f));
            highlight.setPosition(boxX, lineY - 5.0f);
            highlight.setFillColor(sf::Color(255, 228, 181)); // Màu vàng highlight
            window.draw(highlight);
        }

        sf::Text text;
        text.setFont(app->font); 
        text.setString((*codePtr)[i]);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::Black);
        text.setPosition(boxX + 15.0f, lineY);
        window.draw(text);
    }
}