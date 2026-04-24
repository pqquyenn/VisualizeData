# Data Structures and Algorithms Visualization

## 📖 Overview
This project is a visualization application that simulates the operations of popular data structures and algorithms. It is written in **C++** and uses **CMake** for build management.

The goal of this project is to help students and programming enthusiasts easily understand how algorithms work step-by-step through an intuitive graphical interface.

## ✨ Key Features

Currently, the application supports visualizing the following Data Structure and Algorithms

* **Lists :** Singly linked list
* **Tree :** AVL Tree, AAtree
* **Priority queue :** Heap
* **Graph Algorithm :** Dijkstra 's Shortest Path Algorithm
* Minimum Spanning Tree (MST)

## 📂 Project Structure

The project is organized following modern C++ standards:
```
├───.vscode
├───📂assets  #Contains graphical resources, fonts, etc.
├───📂external # Third-party libraries
├───📂include  # Contains all Header files (*.h)
│   ├───📂AATree
│   ├───📂AVLTree
│   ├───📂Core  # Core components of the app (App, State)
│   ├───📂Dijkstra
│   ├───📂Heap
│   ├───📂MinimumST
│   ├───📂Screen  # Manages the screens for each algorithm
│   ├──📂SinglyLinkedList
│   └───📂UI # User interface components (Button, InputBox,...)
├───📂src  # Contains all Header files (*.h)
│   ├───📂AATree
│   ├───📂AVLTree
│   ├───📂Core  # Core components of the app (App, State)
│   ├───📂Dijkstra
│   ├───📂Heap
│   ├───📂MinimumST
│   ├───📂Screen  # Manages the screens for each algorithm
│   ├───📂SinglyLinkedList
│   └───📂UI # User interface components (Button, InputBox,...)
├── 📜main.cpp
├── 📜CMakeLists.txt
```

