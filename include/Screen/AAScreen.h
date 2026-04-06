#pragma once
#include "State.h"
#include "Button.h"
#include "InputBox.h"
#include "AATree.h"
#include <fstream>
#include <vector>

class App;

class AAScreen : public State {
private:
    Button* btnBackToMenu;
    Button* btnInsert;
    Button* btnSearch;
    Button* btnDelete;
    Button* btnInit;
    Button* btnInitFromFile;

    Button* btnStepBack;
    Button* btnPausePlay;
    Button* btnStepForward;
    Button* btnSpeedDown;
    Button* btnSpeedUp;

    InputBox* inputVal;
    AATree* aaTree;

    sf::View treeView;
    bool isPanning = false;
    sf::Vector2i oldMousePos;
    bool isViewInitialized = false;

    std::vector<std::string> codeInsert = {
        "if (node == NULL) return new Node(key, lvl=1)",
        "if (key < node->key) insert(node->left, key)",
        "else if (key > node->key) insert(node->right, key)",
        "node = skew(node)",
        "node = split(node)"
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
        "decrease_level(node)",
        "skew(node) & skew(node.right) ...",
        "split(node) & split(node.right)"
    };

public:
    AAScreen(App* app);
    ~AAScreen();

    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
    void drawCodeBlock(sf::RenderWindow& window);
};