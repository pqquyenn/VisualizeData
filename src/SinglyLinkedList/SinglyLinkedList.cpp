// #include "SinglyLinkedList.h"
// #include <cmath>

// void SinglyLinkedList::initVariable(sf::Font &font){
//     this->head = nullptr;
//     this->font = font;
//     this->NodeRadius = 30.f;
//     currentstate = SLLState::Idle;
//     this->currentNode = nullptr;
//     timer = 0.f;
//     this->head = new ListNode(5, font, sf::Vector2f (100, 300));
//     this->head->next = new ListNode(10, font, sf::Vector2f(150, 300));
// }

// SinglyLinkedList::SinglyLinkedList(sf::Font& font){
//     this->initVariable(font);
// }

// SinglyLinkedList::~SinglyLinkedList(){
//     this->clear();
// }


// void SinglyLinkedList::drawArrow(sf::RenderWindow &window, sf::Vector2f start, sf::Vector2f end){
//     sf::Vector2f direction = end - start;
//     float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
//     if (length == 0) return;
//     float arrowlength = length - 2*(this->NodeRadius);
//     if (arrowlength < 0){
//         arrowlength = 0;
//         return;
//     }
//     //
//     sf::RectangleShape line(sf::Vector2f(arrowlength, 2.f));
//     float edgeStart = start.x + (this->NodeRadius);
//     line.setPosition(edgeStart, start.y);
//     line.setFillColor(sf::Color::Black);
//     window.draw(line);
// }


// void SinglyLinkedList::clear() {
//     ListNode* current = head;
//     while (current != nullptr) {
//         ListNode* next = current->next;
//         delete current;
//         current = next;
//     }
//     head = nullptr;
//     currentstate = SLLState::Idle;
// }

// void SinglyLinkedList::updatePosition(float deltaTime) {
//     ListNode* temp = head;
//     int index = 0;
    
//     // Giả sử Node đầu tiên nằm ở x = 100, y = 300. Khoảng cách giữa các node là 120px.
//     while (temp != nullptr) {
//         temp->targetPosition = sf::Vector2f(100.f + index * 120.f, 300.f);
//         temp = temp->next;
//         index++;
//     }
//     ListNode* cur = head;
//     while (cur != nullptr) {
//         cur->update(deltaTime);
//         cur = cur->next;
//     }
// }

// void SinglyLinkedList::startSearch(int val) {
//     if (currentstate != SLLState::Idle || head == nullptr) return;
//     targetvalue = val;
//     currentstate = SLLState::Searching;
//     currentNode = head;
//     timer = 0.0f;
//     currentNode->setColor(sf::Color::Green);
// }

// // void SinglyLinkedList::updateSearching(int val, float deltaTime) {
// //     // 1. Logic tìm kiếm (Đổi màu tuần tự)
// //     if (currentstate != SLLState::Idle || head == nullptr) return;
// //     if (currentstate == SLLState::Idle){
// //         targetvalue = val;
// //         currentstate = SLLState::Searching;
// //         currentNode = head;
// //         timer = 0.0f;
// //         currentNode->setColor(sf::Color::Green);
// //     }
    
// //     if (currentstate == SLLState::Searching) {
// //         timer += 1/deltaTime;
// //         if (timer >= 50.f) { // 1 giây chuyển 1 node
// //             timer = 0.0f;
// //             if (currentNode->val == targetvalue) {
// //                 currentNode->setColor(sf::Color::Red);
// //                 currentstate = SLLState::Idle; 
// //             } else {
// //                 currentNode->setColor(sf::Color::White); // Trả lại màu cũ
// //                 currentNode = currentNode->next;
// //                 if (currentNode != nullptr) {
// //                     currentNode->setColor(sf::Color::Green);
// //                 } else {
// //                     currentstate = SLLState::Idle; // Không tìm thấy
// //                 }
// //             }
// //         }
// //     }
    
// //     // 2. Update logic di chuyển mượt (Animation) cho mọi node
// //     ListNode* temp = head;
// //     while (temp != nullptr) {
// //         temp->update(deltaTime);
// //         temp = temp->next;
// //     }
// // }

// void SinglyLinkedList::updateSearching(float deltaTime) {
//     if (head == nullptr) return;
    
//     // Chỉ update logic tìm kiếm khi state đang là Searching
//     if (currentstate == SLLState::Searching) {
//         timer += deltaTime; // Cộng dồn thời gian thực tế (giây)
        
//         if (timer >= 1.0f) { // Cứ 1.0 giây sẽ nhảy 1 node
//             timer = 0.0f; // Reset timer
            
//             if (currentNode->val == targetvalue) {
//                 currentNode->setColor(sf::Color::Red); // Tìm thấy
//                 currentstate = SLLState::Idle; 
//             } else {
//                 // Trả lại màu gốc (Xanh dương đậm) thay vì màu Trắng
//                 currentNode->setColor(sf::Color(7, 67, 102)); 
                
//                 currentNode = currentNode->next; // Nhảy sang node tiếp theo
                
//                 if (currentNode != nullptr) {
//                     currentNode->setColor(sf::Color::Green); // Đổi màu node mới
//                 } else {
//                     currentstate = SLLState::Idle; // Hết danh sách, không tìm thấy
//                 }
//             }
//         }
//     }
    
//     // Update logic di chuyển mượt (Animation) cho mọi node
//     ListNode* temp = head;
//     while (temp != nullptr) {
//         temp->update(deltaTime);
//         temp = temp->next;
//     }
// }

// void SinglyLinkedList::draw(sf::RenderWindow& window) {
//     ListNode* temp = head;
    
//     // Bước 1: Vẽ các mũi tên trước (để mũi tên nằm dưới các Node)
//     while (temp != nullptr && temp->next != nullptr) {
//         drawArrow(window, temp->shape.getPosition(), temp->next->shape.getPosition());
//         temp = temp->next;
//     }

//     // Bước 2: Vẽ các Node (Box + Text) đè lên
//     temp = head;
//     while (temp != nullptr) {
//         temp->renderNode(window);
//         temp = temp->next;
//     }
// }

#include "SinglyLinkedList.h"
#include <iostream>

// Khởi tạo danh sách liên kết
SinglyLinkedList::SinglyLinkedList(sf::Font& font) {
    this->font = font;
    this->head = nullptr;
    this->currentstate = SLLState::Idle;
    this->currentNode = nullptr;
    this->NodeRadius = 30;
    this->targetvalue = -1;
    this->timer = 0.0f;
}

// Hàm hủy để dọn dẹp bộ nhớ
SinglyLinkedList::~SinglyLinkedList() {
    clear();
}

// Xóa toàn bộ Node
void SinglyLinkedList::clear() {
    ListNode* current = head;
    while (current != nullptr) {
        ListNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = nullptr;
}

// Bắt đầu sự kiện tìm kiếm từ InputBox
void SinglyLinkedList::startSearch(int val) {
    this->targetvalue = val;
    this->currentstate = SLLState::Searching;
    this->currentNode = head;
    this->timer = 0.0f;
    
    // In tạm ra console để biết nút Search đã hoạt động
    std::cout << "Dang tim kiem gia tri: " << val << "\n";
}

// Các hàm update và draw tạm thời để nguyên khung xương
void SinglyLinkedList::updateSearching(float deltaTime) {
    // Tạm thời để trống, sẽ thêm logic chuyển màu sau
}

void SinglyLinkedList::updatePosition(float deltaTime) {
    ListNode* current = head;
    while (current != nullptr) {
        current->update(deltaTime);
        current = current->next;
    }
}

void SinglyLinkedList::draw(sf::RenderWindow& window) {
    ListNode* current = head;
    while (current != nullptr) {
        current->renderNode(window);
        current = current->next;
    }
}

// Các hàm phụ trợ (Tạm để trống để không bị lỗi Linker)
void SinglyLinkedList::initVariable(sf::Font &font) {
}

void SinglyLinkedList::drawArrow(sf::RenderWindow &window, sf::Vector2f start, sf::Vector2f end) {
}