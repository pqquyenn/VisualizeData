#include "PlaybackController.h"

PlaybackController::PlaybackController(float x, float y, sf::Font& font, SinglyLinkedList* list) {
    sll = list;
    // Tạo 3 nút nằm cạnh nhau
    btnStepBack = new Button(x, y, 60, 40, font, "<");
    btnPausePlay = new Button(x + 80, y, 110, 40, font, "Pause/Play");
    btnStepForward = new Button(x + 210, y, 60, 40, font, ">");
}

PlaybackController::~PlaybackController() {
    delete btnStepBack;
    delete btnPausePlay;
    delete btnStepForward;
}

void PlaybackController::handleEvent(sf::Event& event, sf::Vector2f mousePos) {
    if (btnStepBack->isClicked(event, mousePos)) {
        sll->stepBackward();
    }
    if (btnPausePlay->isClicked(event, mousePos)) {
        sll->togglePause();
    }
    if (btnStepForward->isClicked(event, mousePos)) {
        sll->stepForward();
    }
}

void PlaybackController::update(sf::Vector2f mousePos) {
    btnStepBack->update(mousePos);
    btnPausePlay->update(mousePos);
    btnStepForward->update(mousePos);
}

void PlaybackController::draw(sf::RenderWindow& window) {
    btnStepBack->draw(window);
    btnPausePlay->draw(window);
    btnStepForward->draw(window);
}