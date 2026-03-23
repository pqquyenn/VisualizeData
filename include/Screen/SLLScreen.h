#pragma once
#include "State.h"
#include "Button.h"
#include "InputBox.h"
#include "SinglyLinkedList.h" // Nhớ include thư viện này

class App;

class SLLScreen : public State {
private:
    Button* btnBack;
    Button* btnInsert;
    InputBox* inputVal;
    SinglyLinkedList* sll; // Khai báo danh sách liên kết ở đây

public:
    SLLScreen(App* app);
    ~SLLScreen();

    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
};