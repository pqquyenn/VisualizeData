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

## 🛠️ Prerequisites

To build and run this project on your local machine, you need to install:

* C++ Compiler: Supports C++17 or higher (GCC, Clang, or MSVC).

* CMake: Version 3.10 or higher.

* SFML Library: The graphical library used for the UI. (For Windows, it is highly recommended to install GCC, CMake, and SFML via MSYS2 / MinGW-w64 using pacman).

Intructions of storing Cmake and SFML from MinGW Environment

1.  **Install MSYS2:** Download and install MSYS2 from [https://www.msys2.org/](https://www.msys2.org/).
2. **Install required tools:** Open the **MSYS2 MinGW 64-bit** terminal (make sure to select the correct 64-bit version) and run the following commands:
   
   Update the system:
   ```bash
   pacman -Syu

Install C++ Compiler (GCC), Make, and CMake:
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make mingw-w64-x86_64-cmake
```
Install the SFML graphics library:
```bash
pacman -S mingw-w64-x86_64-sfml
```
## 🚀 Build Instructions

Follow these steps to clone and run the project locally:

**Step 1**: Clone the repository

```bash
git clone [https://github.com/pqquyenn/VisualizeData.git](https://github.com/pqquyenn/VisualizeData.git)
cd VisualizeData
```

**Step 2**: Create a build directory and configure CMake
```bash
mkdir build
cd build
cmake ..
```

Step 3: Build the project
```bash
# Đối với Linux/macOS
make

# Hoặc dùng lệnh chung của CMake cho mọi hệ điều hành
cmake --build .
```
Step 4: Run the application
```bash
./VisualizeData.exe
```
🎮 Usage

After launching the application, the Main Menu will appear. You can:

1. Click on the corresponding buttons to select the algorithm or data structure you want to view.

2. Use the input boxes to add, delete, or search for values.

3. Observe the step-by-step simulation displayed on the screen.

🤝 Contributing
Contributions to improve this project are always welcome! If you want to add a new algorithm or fix a bug, please:

1. Fork this repository.

2. Create a new branch (git checkout -b feature/NewFeatureName).

3. Commit your changes (git commit -m 'Add some feature').

4. Push to the branch (git push origin feature/NewFeatureName).

5. Open a Pull Request.
