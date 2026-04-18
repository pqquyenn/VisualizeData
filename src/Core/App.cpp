#include "App.h"
#include "MainMenu.h"
#include "State.h"
#include <iostream>

App::App() {
    window.create(sf::VideoMode(1280, 720), "DSVisual - Data Structure Visualizer");
    window.setFramerateLimit(60);

    if (!font.loadFromFile("../assets/ARIAL.TTF")) {
        std::cerr << "Loi load font!\n" << std::endl;
    }

    currentState = new MainMenu(this);
    nextState = nullptr; 
}

App::~App() {
    if (currentState) delete currentState;
    if (nextState) delete nextState;
}

void App::changeState(State* newState) {
    nextState = newState; 
}

void App::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (currentState) currentState->handleEvent(event, window);
        }

        if (currentState) currentState->update(deltaTime, window);

        window.clear(sf::Color(40, 44, 52));
        if (currentState) currentState->draw(window);
        window.display();

        if (nextState != nullptr) {
            if (currentState) delete currentState; 
            currentState = nextState;              
            nextState = nullptr;                   
        }
    }
}