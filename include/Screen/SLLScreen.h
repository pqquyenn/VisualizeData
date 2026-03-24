#pragma once
#include "State.h"
#include "Button.h"
#include "InputBox.h"
#include "SinglyLinkedList.h"

class App;

class SLLScreen : public State {
private:
    Button* btnBackToMenu;
    Button* btnInsert;
    Button* btnSearch;
    Button* btnDelete;
    Button* btnInit;

    // Tích hợp trực tiếp Playback Buttons (Không dùng PlaybackController)
    Button* btnStepBack;
    Button* btnPausePlay;
    Button* btnStepForward;
    Button* btnSpeedDown;
    Button* btnSpeedUp;

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