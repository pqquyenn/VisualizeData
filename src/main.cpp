#include "ListNode.h"
#include <iostream>
#include "SinglyLinkedList.h"
#include "App.h"
using namespace std;
    // sf::Font font;
    // if(!font.loadFromFile("ARIAL.TTF")){
    //     std::cerr << "Loi: Khong the tai font chu! Vui long dam bao ban co file arial.ttf" << std::endl;
    //     return -1;
    // }
    // sf::RenderWindow window(sf::VideoMode(2000, 1000), "Data Structure", sf::Style::Default);
    // sf::Event ev;
    // SinglyLinkedList a(font);
    // window.setFramerateLimit(60); // Giới hạn 60 FPS
    // sf::Clock clock; // Tạo đồng hồ để đo deltaTime
    // a.startSearch(11);
    // float deltaTime = 1.0/60.f;
    // while (window.isOpen()){
    //     // Lấy thời gian trôi qua giữa 2 frame (tính bằng giây, thường ~0.016s)
    //     // float deltaTime = clock.restart().asSeconds(); 

    //     while (window.pollEvent(ev)){
    //         if (ev.type == sf::Event::Closed){
    //             window.close();
    //             break;
    //         }
    //         if (ev.type == sf::Event::Resized)
    //         {
    //             sf::FloatRect visibleArea(0.f, 0.f, ev.size.width, ev.size.height);
    //             window.setView(sf::View(visibleArea));
    //         }
    //     }
    //     window.clear(sf::Color::White);
        
    //     a.updatePosition(deltaTime);
    //     a.updateSearching(deltaTime); // Update bộ đếm thời gian đổi màu
        
    //     a.draw(window);
    //     window.display();
    // }

int main() {
    App app;
    app.run();
    return 0;
}
