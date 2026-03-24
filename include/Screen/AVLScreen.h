#pragma once
#include "State.h"
#include "Button.h"
#include "InputBox.h"
#include "AVLTree.h"

class App;

class AVLScreen : public State {
private:
    Button* btnBackToMenu;
    Button* btnInsert;
    Button* btnSearch;
    Button* btnDelete;
    Button* btnInit;

    // Các nút Playback được tích hợp thẳng vào đây để không lệ thuộc SinglyLinkedList
    Button* btnStepBack;
    Button* btnPausePlay;
    Button* btnStepForward;
    Button* btnSpeedDown;
    Button* btnSpeedUp;

    InputBox* inputVal;
    AVLTree* avlTree;

    sf::View treeView;
    bool isPanning = false;
    sf::Vector2i oldMousePos;
    bool isViewInitialized = false;

public:
    AVLScreen(App* app);
    ~AVLScreen();

    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
};