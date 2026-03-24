#pragma once
#include "State.h" // Nhớ trỏ đúng đường dẫn của bạn
#include "Button.h"
#include "InputBox.h"
#include "PlaybackController.h"
#include "AVLTree.h"

class App; // Forward declaration

class AVLScreen : public State {
private:
    Button* btnBackToMenu;
    Button* btnInsert;
    Button* btnSearch;
    Button* btnDelete;
    Button* btnInit;

    InputBox* inputVal;
    
    AVLTree* avlTree;
    PlaybackController* playbackController;

    // Camera (View) để hỗ trợ kéo thả, zoom cho Cây to
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