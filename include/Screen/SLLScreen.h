#pragma once
#include "State.h"
#include "Button.h"
#include "InputBox.h"
#include "SinglyLinkedList.h"

class App;

class SLLScreen : public State {
private:
    Button* btnBack;
    Button* btnInsert;
    Button* btnSearch;
    Button* btnDelete;
    InputBox* inputVal;
    SinglyLinkedList* sll;

public:
    SLLScreen(App* app);
    ~SLLScreen();

    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
};