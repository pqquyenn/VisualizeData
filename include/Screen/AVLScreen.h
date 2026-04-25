#pragma once
#include "State.h"
#include "Button.h"
#include "InputBox.h"
#include "AVLTree.h"
#include <fstream>


class App;

class AVLScreen : public State {
private:
    Button* btnBackToMenu;
    Button* btnInsert;
    Button* btnSearch;
    Button* btnDelete;
    Button* btnInit;

    // Các nút Playback được tích hợp thẳng vào đây để không lệ thuộc SinglyLinkedList
    Button* btnStepBack;
    Button* btnPausePlay;
    Button* btnStepForward;
    Button* btnSpeedDown;
    Button* btnSpeedUp;

    Button* btnSkipBack;    
    Button* btnSkipForward;

    // THÊM DÒNG NÀY:
    sf::Text textSpeed;

    InputBox* inputVal;
    AVLTree* avlTree;

    sf::View treeView;
    bool isPanning = false;
    sf::Vector2i oldMousePos;
    bool isViewInitialized = false;

    // ... (các nút cũ)
    Button* btnInitFromFile; // THÊM NÚT NÀY

    // ... (các biến view, input giữ nguyên)

    // THÊM CÁC MẢNG PSEUDO-CODE
// CẬP NHẬT LẠI 3 MẢNG CODE NÀY (Đã rút gọn)
    std::vector<std::string> codeInsert = {
        "if (node == NULL) return new Node(key)",
        "if (key < node->key) insert(node->left, key)",
        "else if (key > node->key) insert(node->right, key)",
        "update height & get balance factor",
        "if (balance > 1 && key < left) Right Rotate (LL)",
        "if (balance < -1 && key > right) Left Rotate (RR)",
        "if (balance > 1 && key > left) Left-Right Rotate (LR)",
        "if (balance < -1 && key < right) Right-Left Rotate (RL)"
    };

    std::vector<std::string> codeSearch = {
        "if (node == NULL) return NOT_FOUND",
        "if (node->key == key) return FOUND",
        "if (key < node->key) search(node->left, key)",
        "else search(node->right, key)"
    };

    std::vector<std::string> codeDelete = {
        "if (node == NULL) return",
        "if (key < node->key) delete(node->left, key)",
        "else if (key > node->key) delete(node->right, key)",
        "else { delete current node }",
        "update height & get balance factor",
        "if (balance > 1 && left >= 0) Right Rotate (LL)",
        "if (balance < -1 && right <= 0) Left Rotate (RR)",
        "if (balance > 1 && left < 0) Left-Right Rotate (LR)",
        "if (balance < -1 && right > 0) Right-Left Rotate (RL)"
    };


public:
    AVLScreen(App* app);
    ~AVLScreen();

    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
        // THÊM HÀM VẼ KHUNG CODE

    void drawCodeBlock(sf::RenderWindow& window);
};