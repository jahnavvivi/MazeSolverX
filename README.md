# MazeSolverX: Maze Generator and Solver

## 📖 Introduction
**MazeSolverX** is a comprehensive tool developed as an **Algorithm and Problem Solving Lab** project at **Jaypee Institute of Information Technology**. The project focuses on generating random mazes and solving them using various pathfinding algorithms. It provides a real-time visual demonstration of how different algorithms explore paths, offering a comparative analysis of their efficiency, accuracy, and computational complexity.

## 🚀 Features
* **Real-Time Visualization:** Uses **SFML** graphics to render the grid, pathfinding steps, explored areas, and the final path dynamically.
* **Maze Generation:** Dynamically constructs maze grids using **Depth-First Search (DFS)** with recursive backtracking to ensure perfect mazes (no loops).
* **Execution Time Tracking:** Measures and displays the time taken for both generation and solving operations to compare algorithmic performance.
* **Interactive Menu:** A text-based interface that allows users to generate mazes, select algorithms, and reset the state easily.
* **Modular Design:** Built using Object-Oriented principles, separating maze generation and solving logic for better code readability and extensibility.

## 🧠 Algorithms Implemented

The project implements and compares the following algorithms :

1.  **Recursive Backtracking:** A DFS variation used for generating the maze and solving. It finds a path but not necessarily the shortest one.
2.  **Depth-First Search (DFS):** Explores as deep as possible before backtracking. It is memory efficient but does not guarantee the shortest path.
3.  **Breadth-First Search (BFS):** Explores level by level using a Queue. It guarantees the shortest path in unweighted graphs.
4.  **Dijkstra's Algorithm:** Similar to BFS but designed for weighted graphs (adapted here for grid traversal). Guarantees the shortest path.
5.  **Greedy Best-First Search (GBFS):** A heuristic algorithm that selects the node appearing closest to the goal. It is fast but can get stuck in "bad" directions.
6.  **A\* Search Algorithm:** Uses a heuristic function ($f(n) = g(n) + h(n)$) to combine actual cost and estimated cost. It is fast and accurate.
7.  **Bidirectional Search:** Runs two simultaneous searches (one from start, one from goal) and stops when they meet, drastically reducing search space.
8.  **Wall Following:** A simple heuristic (keeping a hand on the wall) that works well in simple, connected mazes.

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
    Once running, use the terminal to control the application:
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
