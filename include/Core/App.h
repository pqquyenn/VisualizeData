#pragma once
#include <SFML/Graphics.hpp>

class State; // Forward declaration

class App {
public:
    sf::RenderWindow window;
    sf::Font font;
    State* currentState;
    State* nextState;

    App();
    ~App();
    
    void changeState(State* newState);
    void run();
};