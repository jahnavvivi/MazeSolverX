# MazeSolverX: Maze Generator and Solver

![Language](https://img.shields.io/badge/Language-C++-blue.svg)
![Library](https://img.shields.io/badge/Library-SFML-green.svg)
![Type](https://img.shields.io/badge/Type-Academic_Project-orange.svg)

## 📖 Introduction
[cite_start]**MazeSolverX** is a comprehensive tool developed as an **Algorithm and Problem Solving Lab** project at **Jaypee Institute of Information Technology**[cite: 1, 3, 11]. The project focuses on generating random mazes and solving them using various pathfinding algorithms. [cite_start]It provides a real-time visual demonstration of how different algorithms explore paths, offering a comparative analysis of their efficiency, accuracy, and computational complexity[cite: 29, 33].

## 🚀 Features
* [cite_start]**Real-Time Visualization:** Uses **SFML** graphics to render the grid, pathfinding steps, explored areas, and the final path dynamically[cite: 36, 49].
* [cite_start]**Maze Generation:** Dynamically constructs maze grids using **Depth-First Search (DFS)** with recursive backtracking to ensure perfect mazes (no loops)[cite: 36, 88].
* [cite_start]**Execution Time Tracking:** Measures and displays the time taken for both generation and solving operations to compare algorithmic performance[cite: 52].
* [cite_start]**Interactive Menu:** A text-based interface that allows users to generate mazes, select algorithms, and reset the state easily[cite: 54, 55].
* [cite_start]**Modular Design:** Built using Object-Oriented principles, separating maze generation and solving logic for better code readability and extensibility[cite: 57, 58].

## 🧠 Algorithms Implemented

[cite_start]The project implements and compares the following algorithms [cite: 38-46]:

1.  **Recursive Backtracking:** A DFS variation used for generating the maze and solving. [cite_start]It finds a path but not necessarily the shortest one [cite: 86-89].
2.  **Depth-First Search (DFS):** Explores as deep as possible before backtracking. [cite_start]It is memory efficient but does not guarantee the shortest path [cite: 80-84].
3.  **Breadth-First Search (BFS):** Explores level by level using a Queue. [cite_start]It guarantees the shortest path in unweighted graphs [cite: 73-77].
4.  **Dijkstra's Algorithm:** Similar to BFS but designed for weighted graphs (adapted here for grid traversal). [cite_start]Guarantees the shortest path [cite: 104-107].
5.  **Greedy Best-First Search (GBFS):** A heuristic algorithm that selects the node appearing closest to the goal. [cite_start]It is fast but can get stuck in "bad" directions [cite: 109-113].
6.  **A\* Search Algorithm:** Uses a heuristic function ($f(n) = g(n) + h(n)$) to combine actual cost and estimated cost. [cite_start]It is fast and accurate [cite: 90-95].
7.  [cite_start]**Bidirectional Search:** Runs two simultaneous searches (one from start, one from goal) and stops when they meet, drastically reducing search space [cite: 97-102].
8.  [cite_start]**Wall Following:** A simple heuristic (keeping a hand on the wall) that works well in simple, connected mazes [cite: 114-117].

## 🛠️ Prerequisites

To run this project, you need:
* **C++ Compiler** (e.g., g++, clang)
* **SFML Library** (Simple and Fast Multimedia Library)

### Installing SFML
* **Ubuntu/Debian:** `sudo apt-get install libsfml-dev`
* **MacOS (Homebrew):** `brew install sfml`
* **Windows:** Download from [SFML-dev.org](https://www.sfml-dev.org/download.php)

## 💻 Compilation and Usage

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/your-username/MazeSolverX.git](https://github.com/your-username/MazeSolverX.git)
    cd MazeSolverX
    ```

2.  **Compile the source code:**
    *(Note: Adjust the command based on your specific file structure)*
    ```bash
    g++ main.cpp -o mazeSolver -lsfml-graphics -lsfml-window -lsfml-system
    ```

3.  **Run the application:**
    ```bash
    ./mazeSolver
    ```

4.  **Interactive Menu:**
    [cite_start]Once running, use the terminal to control the application [cite: 122-139]:
    * **0:** Generate Maze (Must be done first)
    * **1:** Recursive Backtracking
    * **2:** Depth-First Search (DFS)
    * **3:** Breadth-First Search (BFS)
    * **4:** Dijkstra's Algorithm
    * **5:** Greedy Best-First Search (GBFS)
    * **6:** Wall Following
    * **7:** A* Search
    * **8:** Bidirectional Search
    * **9:** Exit
