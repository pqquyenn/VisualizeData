#pragma once
#include "State.h"
#include "Button.h"
#include "InputBox.h"
#include "SinglyLinkedList.h"
#include "PlaybackController.h"

class App;

class SLLScreen : public State {
private:
    Button* btnBackToMenu;
    Button* btnInsert;
    Button* btnSearch;
    Button* btnDelete;
    Button* btnInit;

    InputBox* inputVal;
    SinglyLinkedList* sll;
    PlaybackController* playbackController; // Thêm controller mới

public:
    SLLScreen(App* app);
    ~SLLScreen();

    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
};