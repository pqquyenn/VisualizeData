#pragma once
#include <SFML/Graphics.hpp>

class App; // Forward declaration

class State {
protected:
    App* app;

public:
    // LƯU Ý KỸ: Ở đây chỉ kết thúc bằng dấu chấm phẩy (;), tuyệt đối KHÔNG có {}
    State(App* app);
    virtual ~State();

    virtual void handleEvent(sf::Event& event, sf::RenderWindow& window) = 0;
    virtual void update(float deltaTime, sf::RenderWindow& window) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};