#pragma once
#include "State.h"
#include "Button.h"

class MainMenu : public State {
private:
    Button* btnSLL;
    Button* btnAVL;
    Button* btnHeap;
    Button* btnAA;
    Button* btnMST;
public:
    MainMenu(App* app);
    ~MainMenu();

    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
};