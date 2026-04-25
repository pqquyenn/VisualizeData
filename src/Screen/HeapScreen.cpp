#include "HeapScreen.h"
#include "App.h"
#include "MainMenu.h"
#include <iostream>
#include <fstream>

// THÊM 2 THƯ VIỆN NÀY ĐỂ FORMAT SỐ THẬP PHÂN
#include <sstream>
#include <iomanip>

HeapScreen::HeapScreen(App* app) : State(app), isPanning(false), isViewInitialized(false) {
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        if (!font.loadFromFile("external/ARIAL.TTF")) {
             std::cerr << "Khong the load font arial!\n";
        }
    }

    heapTree = new HeapTree(font);

    btnBackToMenu = new Button(20, 20, 110, 40, app->font, "Back Menu");
    inputVal = new InputBox(150, 80, 100, 40, app->font, 3);
    
    btnInsert = new Button(270, 80, 100, 40, app->font, "Insert");
    btnSearch = new Button(390, 80, 100, 40, app->font, "Search");
    btnDeleteMax = new Button(510, 80, 100, 40, app->font, "Delete");
    btnInit= new Button(630, 80, 100, 40, app->font, "Init");
    btnInitFromFile = new Button(750, 80, 100, 40, app->font, "Init File");

    btnSpeedDown   = new Button(150, 140, 50, 40, app->font, "<<");
    btnSkipBack    = new Button(190 + 20, 140, 40, 40, app->font, "|<"); // Nút mới
    btnStepBack    = new Button(240 + 20, 140, 40, 40, app->font, "<");
    btnPausePlay   = new Button(290 + 20, 140, 120, 40, app->font, "Pause/Play");
    btnStepForward = new Button(420 + 20, 140, 40, 40, app->font, ">");
    btnSkipForward = new Button(470 + 20, 140, 40, 40, app->font, ">|"); // Nút mới
    btnSpeedUp     = new Button(520 + 20, 140, 50, 40, app->font, ">>");

    textSpeed.setFont(app->font);
    textSpeed.setCharacterSize(16);
    textSpeed.setFillColor(sf::Color::White);
    // Vị trí X = 600 để nằm bên phải nút ">>" (vì X của nút là 540, rộng 50)
    textSpeed.setPosition(610, 150);

    treeView.setSize(1280, 720); 
    uiView.setSize(1280, 720);   
    uiView.setCenter(640, 360);
}

HeapScreen::~HeapScreen() {
    delete heapTree; delete btnBackToMenu; delete btnInsert;
    delete btnDeleteMax; delete btnInit; delete inputVal;
    delete btnSpeedDown; delete btnStepBack; delete btnPausePlay;
    delete btnStepForward; delete btnSpeedUp;
    delete btnSkipBack; 
    delete btnSkipForward;
    delete btnInitFromFile; // Nhớ xóa vùng nhớ
    delete btnSearch; // Nhớ xóa nút Search
}

void HeapScreen::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (!isViewInitialized) {
        treeView = window.getDefaultView();
        isViewInitialized = true;
    }

    window.setView(uiView);
    sf::Vector2f mousePosUI = window.mapPixelToCoords(sf::Mouse::getPosition(window), uiView);
    
    inputVal->handleEvent(event, mousePosUI);

    // Bắt sự kiện click nút Search
    if (btnSearch->isClicked(event, mousePosUI)) {
        std::string valStr = inputVal->getText();
        if (!valStr.empty()) {
            heapTree->searchVal(std::stoi(valStr));
            // Không clear inputVal ở đây để người dùng biết mình vừa search số mấy
        }
    }
    // ... (code cũ: xử lý click btnInsert, btnDeleteMax...)
    if (btnBackToMenu->isClicked(event, mousePosUI)) {
        app->changeState(new MainMenu(app));
        return;
    }
    if (btnInsert->isClicked(event, mousePosUI)) {
        std::string valStr = inputVal->getText();
        if (!valStr.empty()) {
            heapTree->insertVal(std::stoi(valStr));
            inputVal->clear();
        }
    }
    if (btnDeleteMax->isClicked(event, mousePosUI)) { 
        heapTree->deleteMax(); 
    }
    if (btnInit->isClicked(event, mousePosUI)) {
        std::string valStr = inputVal->getText();
        int n = valStr.empty() ? 10 : std::stoi(valStr);
        heapTree->initTree(n);
    }

    if (btnInitFromFile->isClicked(event, mousePosUI)) {
        // Đọc từ file input.txt cùng thư mục chứa file .exe
        std::ifstream file("input.txt");
        if (file.is_open()) {
            std::vector<int> data;
            int val;
            while (file >> val) {
                data.push_back(val);
            }
            file.close();
            if (!data.empty()) {
                heapTree->initFromFile(data);
            }
        } else {
            std::cout << "Khong tim thay file input.txt!\n";
        }
    }
    
    if (btnPausePlay->isClicked(event, mousePosUI)) heapTree->togglePause();
    if (btnStepBack->isClicked(event, mousePosUI)) heapTree->stepBackward();
    if (btnStepForward->isClicked(event, mousePosUI)) heapTree->stepForward();
    if (btnSpeedUp->isClicked(event, mousePosUI)) heapTree->increaseSpeed();
    if (btnSpeedDown->isClicked(event, mousePosUI)) heapTree->decreaseSpeed();
    if (btnSkipForward->isClicked(event, mousePosUI)) heapTree->skipToLastStep();
    if (btnSkipBack->isClicked(event, mousePosUI)) heapTree->skipToFirstStep();

    window.setView(treeView);
    
    heapTree->handleEvent(event, window, treeView);

    if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
        if (event.mouseWheelScroll.delta > 0) treeView.zoom(0.9f);
        else treeView.zoom(1.1f);
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        if (pixelPos.y > 200 && !heapTree->isDraggingNode()) {
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
}

void HeapScreen::update(float deltaTime, sf::RenderWindow& window) {
    sf::Vector2f mousePosUI = window.mapPixelToCoords(sf::Mouse::getPosition(window), uiView);
    
    btnBackToMenu->update(mousePosUI);
    btnSearch->update(mousePosUI); // Cập nhật nút Search
    btnInsert->update(mousePosUI);
    btnDeleteMax->update(mousePosUI); 
    btnInit->update(mousePosUI);
    btnSpeedDown->update(mousePosUI); 
    btnStepBack->update(mousePosUI); 
    btnPausePlay->update(mousePosUI);
    btnStepForward->update(mousePosUI); 
    btnSpeedUp->update(mousePosUI);
    btnSkipBack->update(mousePosUI); 
    btnSkipForward->update(mousePosUI);

    float currentSpeed = heapTree->getSpeedMultiplier();
    std::stringstream ss;
    ss << "Speed: " << std::fixed << std::setprecision(2) << currentSpeed << "x";
    textSpeed.setString(ss.str());

    heapTree->updateAnimation(deltaTime);
    heapTree->updatePosition(deltaTime); 

    btnInitFromFile->update(mousePosUI); // Cập nhật trạng thái hover nút
}

void HeapScreen::draw(sf::RenderWindow& window) {
    window.clear(sf::Color(40, 45, 52)); 

    if (isViewInitialized) window.setView(treeView);
    heapTree->draw(window);

    window.setView(uiView); 

    // ... code cũ gọi các hàm draw()
    btnInitFromFile->draw(window); // Vẽ nút mới
    // 3. Gọi hàm vẽ khung code
    drawCodeBlock(window);
    btnBackToMenu->draw(window); inputVal->draw(window); 
    btnInsert->draw(window); btnDeleteMax->draw(window); btnInit->draw(window);
    btnSearch->draw(window); // Vẽ nút Search
    btnSpeedDown->draw(window); btnStepBack->draw(window); btnPausePlay->draw(window);
    btnStepForward->draw(window); btnSpeedUp->draw(window);
    btnSkipBack->draw(window); 
    btnSkipForward->draw(window);
    // THÊM VÀO DÒNG NÀY Ở CUỐI CÙNG:
    window.draw(textSpeed);
}

// 4. Cài đặt hàm vẽ khung Code góc dưới bên phải
void HeapScreen::drawCodeBlock(sf::RenderWindow& window) {
    std::string op = heapTree->getCurrentOperation();
    if (op != "Insert" && op != "Delete" && op != "Search") return;

    const std::vector<std::string>* codePtr = &codeInsert;
    if (op == "Delete") codePtr = &codeDelete;
    else if (op == "Search") codePtr = &codeSearch;

    int activeLine = heapTree->getCurrentActiveLine();

    // Thông số khung nền màu be (giống trong ảnh Youtube của bạn)
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

        // Nếu là dòng đang chạy (activeLine), vẽ một dải màu vàng làm highlight
        if (i == activeLine) {
            sf::RectangleShape highlight(sf::Vector2f(boxWidth, 30.0f));
            highlight.setPosition(boxX, lineY - 5.0f);
            highlight.setFillColor(sf::Color(255, 228, 181)); // Màu vàng highlight
            window.draw(highlight);
        }

        sf::Text text;
        text.setFont(font); // Dùng font đang có của class
        text.setString((*codePtr)[i]);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::Black);
        text.setPosition(boxX + 15.0f, lineY);
        window.draw(text);
    }
}