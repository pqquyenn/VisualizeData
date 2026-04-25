#pragma once

#include "State.h"
#include "Button.h"
#include "InputBox.h"
#include "SinglyLinkedList.h"

class App;

class SLLScreen : public State {
private:
    Button* btnBackToMenu;
    Button* btnInsert;      // (Insert Tail)
    Button* btnSearch;
    Button* btnDelete;
    Button* btnInit;
    Button* btnInitFile;

    // --- CÁC NÚT MỚI ---
    Button* btnInsertHead;
    Button* btnInsertIndex;
    InputBox* inputIndex;      // Ô nhập vị trí (Index)
    Button* btnGoInsertIndex;  // Nút Go
    bool showIndexInput = false; // Trạng thái ẩn/hiện ô nhập Index

    Button* btnStepBack;
    Button* btnPausePlay;
    Button* btnStepForward;
    Button* btnSpeedDown;
    Button* btnSpeedUp;

    Button* btnSkipBack;    
    Button* btnSkipForward;

    InputBox* inputVal;
    SinglyLinkedList* sll;

    sf::View listView;
    bool isPanning = false;
    sf::Vector2i oldMousePos;
    bool isViewInitialized = false;

public:
    SLLScreen(App* app);
    ~SLLScreen();

    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
};