#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "SinglyLinkedList.h"

class PlaybackController {
private:
    Button* btnStepBack;
    Button* btnPausePlay;
    Button* btnStepForward;
    SinglyLinkedList* sll;

public:
    PlaybackController(float x, float y, sf::Font& font, SinglyLinkedList* list);
    ~PlaybackController();

    void handleEvent(sf::Event& event, sf::Vector2f mousePos);
    void update(sf::Vector2f mousePos);
    void draw(sf::RenderWindow& window);
};