#include <SFML/Graphics.hpp>
#include<bits/stdc++.h>
#include <unistd.h>

using namespace std;
using namespace sf;

const int WIDTH = 25;
const int HEIGHT = 25;
const int CELL_SIZE = 20;

int dx[] = {0, 0, -1, 1};
int dy[] = {-1, 1, 0, 0};
int movex[] = {0, 0, -1, 1};
int movey[] = {-1, 1, 0, 0};

struct Node {
    int x, y;
    int cost;
    bool operator>(const Node& other) const {
        return cost > other.cost;
    }
};

struct NodeGBFS {
    int x, y;
    int h;
    vector<pair<int, int>> path;

    bool operator>(const NodeGBFS& other) const {
        return h > other.h;
    }
};

int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

//Maze Generation Class
class MazeGenerator {
public:
    char maze[HEIGHT][WIDTH];

    MazeGenerator() {
        reset();
    }

    void reset() {
        for (int y = 0; y < HEIGHT; ++y)
            for (int x = 0; x < WIDTH; ++x)
                maze[y][x] = '#';
    }

    bool isValid(int x, int y) {
        return x > 0 && x < WIDTH - 1 && y > 0 && y < HEIGHT - 1;
    }

    void drawMaze(RenderWindow &window) {
        window.clear();
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                RectangleShape cell(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                cell.setFillColor(maze[y][x] == '#' ? Color::Black : Color::White);
                window.draw(cell);
            }
        }
        window.display();
        usleep(30000);
    }

    void generateVisual(int x, int y, RenderWindow &window) {
        maze[y][x] = ' ';
        drawMaze(window);

        int dirs[4] = {0, 1, 2, 3};
        for (int i = 0; i < 4; i++)
            swap(dirs[i], dirs[rand() % 4]);

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[dirs[i]] * 2;
            int ny = y + dy[dirs[i]] * 2;
            if (isValid(nx, ny) && maze[ny][nx] == '#') {
                maze[y + dy[dirs[i]]][x + dx[dirs[i]]] = ' ';
                generateVisual(nx, ny, window);
            }
        }
    }

};

//BFS Maze Solver Class
class BFSMazeSolver {
private:
    bool visited[HEIGHT][WIDTH];
    pair<int, int> parent[HEIGHT][WIDTH];
    RenderWindow &window;
    bool bfsFinished = false;
    char (*maze)[WIDTH];

    bool isValid(int x, int y) {
        return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
    }

    void visualize(int currX, int currY) {
        if (bfsFinished) return;
        window.clear();
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                RectangleShape cell(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                if (maze[y][x] == '#')
                    cell.setFillColor(Color::Black);
                else if (visited[y][x])
                    cell.setFillColor(Color(219, 205, 240));
                else
                    cell.setFillColor(Color::White);
                window.draw(cell);
            }
        }

        RectangleShape current(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
        current.setPosition(currX * CELL_SIZE, currY * CELL_SIZE);
        current.setFillColor(Color(219, 205, 240));
        window.draw(current);
        window.display();
        usleep(50000);
    }

public:
    BFSMazeSolver(RenderWindow &win, char m[HEIGHT][WIDTH]) : window(win), maze(m) {
        for (int y = 0; y < HEIGHT; y++)
            for (int x = 0; x < WIDTH; x++)
                visited[y][x] = false;
    }

    vector<pair<int, int>> solve() {
        queue<pair<int, int>> q;
        q.push({1, 1});
        visited[1][1] = true;
        parent[1][1] = {-1, -1};

        while (!q.empty()) {
            auto [x, y] = q.front();
            q.pop();
            visualize(x, y);

            if (x == WIDTH - 2 && y == HEIGHT - 2) {
                vector<pair<int, int>> path;
                for (auto p = make_pair(x, y); p.first != -1; p = parent[p.second][p.first])
                path.push_back(p);
                reverse(path.begin(), path.end());
                return path;
            }

            for (int i = 0; i < 4; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];
                if (isValid(nx, ny) && maze[ny][nx] == ' ' && !visited[ny][nx]) {
                    visited[ny][nx] = true;
                    parent[ny][nx] = {x, y};
                    q.push({nx, ny});
                }
            }
        }
        return {};
    }

    void drawFinalPath(const vector<pair<int, int>> &path) {
        for (auto [x, y] : path) {
            RectangleShape cell(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
            cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
            cell.setFillColor(Color::Green);
            window.draw(cell);
        }
        window.display();
    }
};

//DFS Maze Solver Class
class DFSMazeSolver {
public:
    DFSMazeSolver(char m[][WIDTH], bool v[][WIDTH], RenderWindow& win)
        : maze(m), visited(v), window(win) {}

    void solve() {
        stack<pair<int, int>> s;
        s.push({1, 1});
        visited[1][1] = true;
        parent[1][1] = {-1, -1};

        while (!s.empty()) {
            auto [x, y] = s.top();
            s.pop();
            visualize(x, y);

            if (x == WIDTH - 2 && y == HEIGHT - 2) {
                constructPath(x, y);
                return;
            }

            for (int i = 0; i < 4; i++) {
                int nx = x + movex[i];
                int ny = y + movey[i];
                if (maze[ny][nx] == ' ' && !visited[ny][nx]) {
                    visited[ny][nx] = true;
                    parent[ny][nx] = {x, y};
                    s.push({nx, ny});
                }
            }
        }
    }

    void drawFinalPath() {
        for (auto [x, y] : finalPath) {
            RectangleShape cell(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
            cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
            cell.setFillColor(Color::Green);
            window.draw(cell);
        }
        window.display();
    }

private:
    char (*maze)[WIDTH];
    bool (*visited)[WIDTH];
    RenderWindow& window;

    pair<int, int> parent[HEIGHT][WIDTH];
    vector<pair<int, int>> finalPath;

    void visualize(int currX, int currY) {
        window.clear();
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                RectangleShape cell(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                if (maze[y][x] == '#')
                    cell.setFillColor(Color::Black);
                else if (visited[y][x])
                    cell.setFillColor(Color(242, 198, 222));
                else
                    cell.setFillColor(Color::White);
                window.draw(cell);
            }
        }
        RectangleShape current(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
        current.setPosition(currX * CELL_SIZE, currY * CELL_SIZE);
        current.setFillColor(Color(242, 198, 222));
        window.draw(current);
        window.display();
        usleep(50000);
    }

    void constructPath(int x, int y) {
        while (x != -1 && y != -1) {
            finalPath.push_back({x, y});
            tie(x, y) = parent[y][x];
        }
        reverse(finalPath.begin(), finalPath.end());
    }
};


//Recursive Maze Solver Class
class RecursiveMazeSolver {
public:
    RecursiveMazeSolver(char m[][WIDTH], bool v[][WIDTH], RenderWindow& win)
        : maze(m), visited(v), window(win) {}

    void solve() {
        recursiveSolve(1, 1);
    }

    void drawFinalPath() {
        if (!allPaths.empty()) {
            for (auto [x, y] : allPaths[0]) {
                RectangleShape finalPath(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                finalPath.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                finalPath.setFillColor(Color::Green);
                window.draw(finalPath);
            }
            window.display();
        }
    }

private:
    char (*maze)[WIDTH];
    bool (*visited)[WIDTH];
    RenderWindow& window;

    vector<pair<int, int>> path;
    vector<vector<pair<int, int>>> allPaths;

    void visualize(int currX, int currY) {
        window.clear();

        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                RectangleShape cell(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);

                if (maze[y][x] == '#')
                    cell.setFillColor(Color::Black);
                else if (visited[y][x])
                    cell.setFillColor(Color(249, 198, 201));
                else
                    cell.setFillColor(Color::White);

                window.draw(cell);
            }
        }

        RectangleShape current(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
        current.setPosition(currX * CELL_SIZE, currY * CELL_SIZE);
        current.setFillColor(Color(249, 198, 201));
        window.draw(current);

        window.display();
        usleep(50000);
    }

    void recursiveSolve(int x, int y) {
        if (x == WIDTH - 2 && y == HEIGHT - 2) {
            path.push_back({x, y});
            allPaths.push_back(path);
            return;
        }

        visited[y][x] = true;
        path.push_back({x, y});
        visualize(x, y);

        for (int i = 0; i < 4; i++) {
            int nx = x + movex[i];
            int ny = y + movey[i];
            if (maze[ny][nx] == ' ' && !visited[ny][nx]) {
                recursiveSolve(nx, ny);
            }
        }

        path.pop_back();
        visited[y][x] = false;
        visualize(x, y);
    }
};

//Dijkstra Maze Solver Class
class DijkstraSolver {
    private:
        const vector<vector<int>>& maze;
        vector<vector<bool>> visited;
        vector<pair<int, int>> parent;
        vector<vector<int>> dist;

        RenderWindow& window;

        const int dx[4] = {0, 1, 0, -1};
        const int dy[4] = {1, 0, -1, 0};

    public:
        DijkstraSolver(RenderWindow& win, const vector<vector<int>>& m)
            : window(win), maze(m) {
            visited = vector<vector<bool>>(HEIGHT, vector<bool>(WIDTH, false));
            parent = vector<pair<int, int>>(HEIGHT * WIDTH);
            dist = vector<vector<int>>(HEIGHT, vector<int>(WIDTH, INT_MAX));
        }

        void visualize(int currX, int currY, bool showFinal = false, vector<pair<int, int>>* finalPath = nullptr) {
            window.clear();
            for (int y = 0; y < HEIGHT; ++y) {
                for (int x = 0; x < WIDTH; ++x) {
                    RectangleShape cell(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                    cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);

                    if (maze[y][x] == 1)
                        cell.setFillColor(Color::Black);
                    else if (visited[y][x])
                        cell.setFillColor(Color(198, 222, 241));
                    else
                        cell.setFillColor(Color::White);

                    window.draw(cell);
                }
            }

            if (showFinal && finalPath) {
                for (auto [x, y] : *finalPath) {
                    RectangleShape cell(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                    cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                    cell.setFillColor(Color::Green);
                    window.draw(cell);
                }
            }

            if (currX != -1 && currY != -1) {
                RectangleShape curr(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                curr.setPosition(currX * CELL_SIZE, currY * CELL_SIZE);
                curr.setFillColor(Color(198, 222, 241));
                window.draw(curr);
            }

            window.display();
            usleep(50000);
        }

        void run(int startX, int startY, int goalX, int goalY) {
            dist[startY][startX] = 0;

            priority_queue<Node, vector<Node>, greater<Node>> pq;
            pq.push({startX, startY, 0});
            parent[startY * WIDTH + startX] = {-1, -1};

            while (!pq.empty()) {
                Node current = pq.top();
                pq.pop();

                if (visited[current.y][current.x]) continue;
                visited[current.y][current.x] = true;

                visualize(current.x, current.y);

                if (current.x == goalX && current.y == goalY) break;

                for (int i = 0; i < 4; ++i) {
                    int nx = current.x + dx[i];
                    int ny = current.y + dy[i];

                    if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT &&
                        maze[ny][nx] == 0 && dist[ny][nx] > current.cost + 1) {
                        dist[ny][nx] = current.cost + 1;
                        parent[ny * WIDTH + nx] = {current.x, current.y};
                        pq.push({nx, ny, dist[ny][nx]});
                    }
                }
            }

            vector<pair<int, int>> path;
            for (pair<int, int> at = {goalX, goalY}; at.first != -1; at = parent[at.second * WIDTH + at.first])
            path.push_back(at);
            reverse(path.begin(), path.end());

            visualize(-1, -1, true, &path);
            sleep(milliseconds(2000));
        }
    };


//Greedy Best First Maze Solver Class
class GreedyBFS {
public:
    MazeGenerator& mazeGen;

    GreedyBFS(MazeGenerator& m) : mazeGen(m) {}

    vector<pair<int, int>> search(RenderWindow& window, int startX, int startY, int goalX, int goalY) {
        vector<vector<bool>> visited(HEIGHT, vector<bool>(WIDTH, false));
        priority_queue<NodeGBFS, vector<NodeGBFS>, greater<NodeGBFS>> pq;

        pq.push({startX, startY, heuristic(startX, startY, goalX, goalY), {{startX, startY}}});
        visited[startY][startX] = true;

        while (!pq.empty()) {
            NodeGBFS current = pq.top();
            pq.pop();

            if (current.x == goalX && current.y == goalY)
                return current.path;

            visualize(window, visited, current.path);

            for (int i = 0; i < 4; i++) {
                int nx = current.x + dx[i];
                int ny = current.y + dy[i];

                if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT &&
                    mazeGen.maze[ny][nx] == ' ' && !visited[ny][nx]) {

                    visited[ny][nx] = true;
                    vector<pair<int, int>> newPath = current.path;
                    newPath.push_back({nx, ny});
                    pq.push({nx, ny, heuristic(nx, ny, goalX, goalY), newPath});
                }
            }
        }

        return {};
    }

    void visualize(RenderWindow& window,
                   const vector<vector<bool>>& visited,
                   const vector<pair<int, int>>& currentPath,
                   const vector<pair<int, int>>& finalPath = {}) {
        window.clear();

        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                RectangleShape cell(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);

                if (mazeGen.maze[y][x] == '#') {
                    cell.setFillColor(Color::Black);
                } else if (!finalPath.empty() &&
                        find(finalPath.begin(), finalPath.end(), make_pair(x, y)) != finalPath.end()) {
                    cell.setFillColor(Color::Green);
                } else if (find(currentPath.begin(), currentPath.end(), make_pair(x, y)) != currentPath.end()) {
                    cell.setFillColor(Color(201, 228, 222));
                } else if (visited[y][x]) {
                    cell.setFillColor(Color(201, 228, 222));
                } else {
                    cell.setFillColor(Color::White);
                }

                window.draw(cell);
            }
        }

        window.display();
        usleep(50000);
    }
};


//A* Maze Solver Class
class AStarSolver {
public:
    AStarSolver(RenderWindow& win, char m[HEIGHT][WIDTH]) : window(win), maze(m) {}

    vector<pair<int, int>> solve(int startX, int startY, int goalX, int goalY) {
        vector<vector<bool>> visited(HEIGHT, vector<bool>(WIDTH, false));
        vector<vector<int>> gCost(HEIGHT, vector<int>(WIDTH, INT_MAX));
        vector<vector<pair<int, int>>> parent(HEIGHT, vector<pair<int, int>>(WIDTH, {-1, -1}));

        auto heuristic = [](int x1, int y1, int x2, int y2) {
            return abs(x1 - x2) + abs(y1 - y2);
        };

        priority_queue<Node, vector<Node>, greater<Node>> openList;
        gCost[startY][startX] = 0;
        openList.push({startX, startY, heuristic(startX, startY, goalX, goalY)});

        while (!openList.empty()) {
            Node current = openList.top();
            openList.pop();

            int x = current.x;
            int y = current.y;

            if (visited[y][x]) continue;
            visited[y][x] = true;

            visualize(x, y, Color(247, 217, 196), visited);

            if (x == goalX && y == goalY) {
                vector<pair<int, int>> path;
                while (parent[y][x] != make_pair(-1, -1)) {
                    path.push_back({x, y});
                    tie(x, y) = parent[y][x];
                }
                reverse(path.begin(), path.end());
                drawFinalPath(path);
                return path;
            }

            for (int i = 0; i < 4; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];

                if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && maze[ny][nx] == ' ' && !visited[ny][nx]) {
                    int newGCost = gCost[y][x] + 1;
                    if (newGCost < gCost[ny][nx]) {
                        gCost[ny][nx] = newGCost;
                        int fCost = newGCost + heuristic(nx, ny, goalX, goalY);
                        openList.push({nx, ny, fCost});
                        parent[ny][nx] = {x, y};
                    }
                }
            }
        }

        return {};
    }

    void drawFinalPath(const vector<pair<int, int>>& path) {
        for (auto [x, y] : path) {
            RectangleShape cell(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
            cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
            cell.setFillColor(Color::Green);
            window.draw(cell);
        }
        window.display();
    }

private:
    RenderWindow& window;
    char (*maze)[WIDTH];
    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {-1, 0, 1, 0};

    void visualize(int currX, int currY, Color color, vector<vector<bool>>& visited) {
        window.clear();
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                RectangleShape cell(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);

                if (maze[y][x] == '#')
                    cell.setFillColor(Color::Black);
                else if (visited[y][x])
                    cell.setFillColor(Color(247, 217, 196));
                else
                    cell.setFillColor(Color::White);

                window.draw(cell);
            }
        }

        RectangleShape current(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
        current.setPosition(currX * CELL_SIZE, currY * CELL_SIZE);
        current.setFillColor(color);
        window.draw(current);

        window.display();
        sleep(milliseconds(50));
    }
};


//Wall Following Maze Solver Class
class WallFollowingSolver {
public:
    WallFollowingSolver(RenderWindow& win, char m[HEIGHT][WIDTH]) : window(win), maze(m) {}

        vector<pair<int, int>> solve(int startX, int startY, int goalX, int goalY) {
        vector<vector<bool>> visited(HEIGHT, vector<bool>(WIDTH, false));
        vector<pair<int, int>> path;

        int directions[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

        int x = startX, y = startY;
        int currentDirection = 0;
        path.push_back({x, y});

        while (x != goalX || y != goalY) {
            visited[y][x] = true;
            int left = (currentDirection + 3) % 4;
            int newX = x + directions[left][0];
            int newY = y + directions[left][1];

            if (isValidMove(newX, newY)) {
                x = newX;
                y = newY;
                currentDirection = left;
                path.push_back({x, y});
            } else {

                newX = x + directions[currentDirection][0];
                newY = y + directions[currentDirection][1];

                if (isValidMove(newX, newY)) {
                    x = newX;
                    y = newY;
                    path.push_back({x, y});
                } else {
                    currentDirection = (currentDirection + 1) % 4;
                }
            }
            visualize(x, y, Color(250, 237, 203), visited);
        }

        drawFinalPath(path);
        return path;
    }

private:
    RenderWindow& window;
    char (*maze)[WIDTH];

    bool isValidMove(int x, int y) {
        return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && maze[y][x] != '#';
    }

    void drawFinalPath(const vector<pair<int, int>>& path) {
        for (auto [x, y] : path) {
            RectangleShape cell(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
            cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
            cell.setFillColor(Color::Green);
            window.draw(cell);
        }
        window.display();
    }

    void visualize(int currX, int currY, Color color, vector<vector<bool>>& visited) {
        window.clear();
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                RectangleShape cell(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                if (maze[y][x] == '#') {
                    cell.setFillColor(Color::Black);
                }
                else if (visited[y][x]) {
                    cell.setFillColor(Color(250, 237, 203));
                }
                else {
                    cell.setFillColor(Color::White);
                }

                window.draw(cell);
            }
        }

        RectangleShape current(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
        current.setPosition(currX * CELL_SIZE, currY * CELL_SIZE);
        current.setFillColor(color);
        window.draw(current);

        window.display();
        sleep(milliseconds(50));
    }
};


//Bidirectional Maze Solver Class
class BidirectionalSolver {
    public:
        BidirectionalSolver(RenderWindow& win, char m[HEIGHT][WIDTH]) : window(win), maze(m) {}

        vector<pair<int, int>> solve(int startX, int startY, int goalX, int goalY) {
            vector<vector<bool>> visitedStart(HEIGHT, vector<bool>(WIDTH, false));
            vector<vector<bool>> visitedGoal(HEIGHT, vector<bool>(WIDTH, false));
            map<pair<int, int>, pair<int, int>> parentStart, parentGoal;
            queue<pair<int, int>> queueStart, queueGoal;

            queueStart.push({startX, startY});
            queueGoal.push({goalX, goalY});
            visitedStart[startY][startX] = true;
            visitedGoal[goalY][goalX] = true;
            parentStart[{startX, startY}] = {-1, -1};
            parentGoal[{goalX, goalY}] = {-1, -1};

            while (!queueStart.empty() && !queueGoal.empty()) {
                if (expandSearch(queueStart, visitedStart, parentStart, visitedGoal)) {
                    auto meetingPoint = queueStart.front();
                    auto path = reconstructPath(parentStart, parentGoal, meetingPoint);
                    drawFinalPath(path);
                    return path;
                }
                if (expandSearch(queueGoal, visitedGoal, parentGoal, visitedStart)) {
                    auto meetingPoint = queueGoal.front();
                    auto path = reconstructPath(parentStart, parentGoal, meetingPoint);
                    drawFinalPath(path);
                    return path;
                }

                visualize(visitedStart, visitedGoal);
            }

            return {};
        }

    private:
        RenderWindow& window;
        char (*maze)[WIDTH];

        bool isValidMove(int x, int y) {
            return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && maze[y][x] != '#';
        }

        bool expandSearch(queue<pair<int, int>>& q, vector<vector<bool>>& visited,
                          map<pair<int, int>, pair<int, int>>& parent,
                          const vector<vector<bool>>& otherVisited) {
            auto [x, y] = q.front();
            q.pop();

            for (int i = 0; i < 4; ++i) {
                int nx = x + dx[i];
                int ny = y + dy[i];

                if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && isValidMove(nx, ny) && !visited[ny][nx]) {
                    visited[ny][nx] = true;
                    parent[{nx, ny}] = {x, y};
                    q.push({nx, ny});

                    if (otherVisited[ny][nx]) {
                        return true;
                    }
                }
            }
            return false;
        }

        vector<pair<int, int>> reconstructPath(
            const map<pair<int, int>, pair<int, int>>& parentStart,
            const map<pair<int, int>, pair<int, int>>& parentGoal,
            pair<int, int> meetingPoint) {
            vector<pair<int, int>> path;

            if (parentStart.find(meetingPoint) == parentStart.end() || parentGoal.find(meetingPoint) == parentGoal.end()) {
                cerr << "Error: Meeting point not found in parent maps!" <<endl;
                return path;
            }

            for (pair<int, int> p = meetingPoint; p != make_pair(-1, -1);) {
                if (parentStart.find(p) == parentStart.end()) {
                    cerr<<"Error: Invalid key in parentStart during reconstruction!"<<endl;
                    break;
                }
                path.push_back(p);
                p = parentStart.at(p);
            }
            reverse(path.begin(), path.end());

            for (pair<int, int> p = parentGoal.at(meetingPoint); p != make_pair(-1, -1);) {
                if (parentGoal.find(p) == parentGoal.end()) {
                    cerr << "Error: Invalid key in parentGoal during reconstruction!"<<endl;
                    break;
                }
                path.push_back(p);
                p = parentGoal.at(p);
            }

            return path;
        }

        void drawFinalPath(const vector<pair<int, int>>& path) {
            for (auto [x, y] : path) {
                RectangleShape cell(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                cell.setFillColor(Color::Green);
                window.draw(cell);
            }
            window.display();
        }

        void visualize(const vector<vector<bool>>& visitedStart, const vector<vector<bool>>& visitedGoal) {
            window.clear();

            for (int y = 0; y < HEIGHT; ++y) {
                for (int x = 0; x < WIDTH; ++x) {
                    RectangleShape cell(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                    cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);

                    if (maze[y][x] == '#') {
                        cell.setFillColor(Color::Black);
                    } else if (visitedStart[y][x] && visitedGoal[y][x]) {
                        cell.setFillColor(Color::Green);
                    } else if (visitedStart[y][x]) {
                        cell.setFillColor(Color(226, 207, 196));
                    } else if (visitedGoal[y][x]) {
                        cell.setFillColor(Color(210, 210, 207));
                    } else {
                        cell.setFillColor(Color::White);
                    }

                    window.draw(cell);
                }
            }

            window.display();
            sleep(milliseconds(50));
        }
};



int main() {
    srand(time(0));
    RenderWindow window(VideoMode(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE), "Maze Generator");

    MazeGenerator mazeGen;
    bool mazeReady = false;

    string t0 = "", t1 = "", t2 = "", t3 = "", t4 = "", t5 = "", t6 = "", t7 = "", t8 = "";
    int choice = -1;

    cout << fixed << setprecision(2);
    while (window.isOpen()) {
        if (choice == -1) {
            cout << "\n==== MENU ==== \t\t\t ==== TIME ====\n";
            if(t0.length() != 0)
                cout << "0 - Generate Maze \t\t "<<t0<<"\n";
            else
                cout << "0 - Generate Maze\n";

            if(t1.length() != 0)
                cout << "1 - Solve with Recursive \t "<<t1<<"\n";
            else
                cout << "1 - Solve with Recursive\n";

            if(t2.length() != 0)
                cout << "2 - Solve with DFS \t\t "<<t2<<"\n";
            else
                cout << "2 - Solve with DFS\n";

            if(t3.length() != 0)
                cout << "3 - Solve with BFS \t\t "<<t3<<"\n";
            else
                cout << "3 - Solve with BFS\n";

            if(t4.length() != 0)
                cout << "4 - Solve with Dijkstra \t "<<t4<<"\n";
            else
                cout << "4 - Solve with Dijkstra\n";

            if(t5.length() != 0)
                cout << "5 - Solve with GBFS \t\t "<<t5<<"\n";
            else
                cout << "5 - Solve with GBFS\n";

            if(t6.length() != 0)
                cout << "6 - Solve with WallFollowing \t "<<t6<<"\n";
            else
                cout << "6 - Solve with WallFollowing\n";

            if(t7.length() != 0)
                cout << "7 - Solve with A* \t\t "<<t7<<"\n";
            else
                cout << "7 - Solve with A*\n";

            if(t8.length() != 0)
                cout << "8 - Solve with Bidirectional \t "<<t8<<"\n";
            else
                cout << "8 - Solve with Bidirectional\n";

            cout << "9 - Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            if (choice == 9) {
                window.close();
                break;
            }
        }

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        if (choice == 0) {
            clock_t start = clock();

            mazeGen.reset();
            mazeGen.generateVisual(1, 1, window);
            mazeReady = true;

            clock_t finish = clock();
            t0 = to_string((double)(finish - start) / CLOCKS_PER_SEC);
            choice = -1;
        }

        else if (!mazeReady) {
            cout << "Please generate a maze first (option 0).\n";
            choice = -1;
        }

        else if (choice == 1) {
            clock_t start = clock();

            bool visited[HEIGHT][WIDTH] = {false};
            RecursiveMazeSolver recSolver(mazeGen.maze, visited, window);
            recSolver.solve();
            recSolver.drawFinalPath();

            clock_t finish = clock();
            t1 = to_string((double)(finish - start) / CLOCKS_PER_SEC);
            choice = -1;
        }

        else if (choice == 2) {
            clock_t start = clock();

            bool visited[HEIGHT][WIDTH] = {false};
            DFSMazeSolver dfs(mazeGen.maze, visited, window);
            dfs.solve();
            dfs.drawFinalPath();

            clock_t finish = clock();
            t2 = to_string((double)(finish - start) / CLOCKS_PER_SEC);
            choice = -1;
        }

        else if (choice == 3) {
            clock_t start = clock();

            BFSMazeSolver bfs(window, mazeGen.maze);
            auto path = bfs.solve();
            bfs.drawFinalPath(path);

            clock_t finish = clock();
            t3 = to_string((double)(finish - start) / CLOCKS_PER_SEC);
            choice = -1;
        }

        else if (choice == 4) {
            clock_t start = clock();

            vector<vector<int>> intMaze(HEIGHT, vector<int>(WIDTH));
            for (int y = 0; y < HEIGHT; ++y)
                for (int x = 0; x < WIDTH; ++x)
                    intMaze[y][x] = (mazeGen.maze[y][x] == '#') ? 1 : 0;

            DijkstraSolver dijkstra(window, intMaze);
            dijkstra.run(1, 1, WIDTH - 2, HEIGHT - 2);

            clock_t finish = clock();
            t4 = to_string((double)(finish - start) / CLOCKS_PER_SEC);
            choice = -1;
        }

        else if (choice == 5) {
            clock_t start = clock();

            GreedyBFS greedyBFS(mazeGen);
            vector<pair<int, int>> path = greedyBFS.search(window, 1, 1, WIDTH - 2, HEIGHT - 2);
            greedyBFS.visualize(window, vector<vector<bool>>(HEIGHT, vector<bool>(WIDTH, false)), path, path);

            clock_t finish = clock();
            t5 = to_string((double)(finish - start) / CLOCKS_PER_SEC);
            choice = -1;
        }

        else if (choice == 6) {
            clock_t start = clock();

            WallFollowingSolver wallFollower(window, mazeGen.maze);
            vector<pair<int, int>> path = wallFollower.solve(1, 1, WIDTH - 2, HEIGHT - 2);

            clock_t finish = clock();
            t6 = to_string((double)(finish - start) / CLOCKS_PER_SEC);
            choice = -1;
        }

        else if (choice == 7) {
            clock_t start = clock();

            AStarSolver astar(window, mazeGen.maze);
            vector<pair<int, int>> path = astar.solve(1, 1, WIDTH - 2, HEIGHT - 2);
            astar.drawFinalPath(path);

            clock_t finish = clock();
            t7 = to_string((double)(finish - start) / CLOCKS_PER_SEC);
            choice = -1;
        }

        else if (choice == 8) {
            clock_t start = clock();

            int startX = 1, startY = 1;
            int goalX = WIDTH - 2, goalY = HEIGHT - 2;
            BidirectionalSolver bidirectionalSolver(window, mazeGen.maze);
            bidirectionalSolver.solve(startX, startY, goalX, goalY);

            clock_t finish = clock();
            t8 = to_string((double)(finish - start) / CLOCKS_PER_SEC);
            choice = -1;
        }

        else if (choice == 9) {
            window.close();
        }
    }

    return 0;
}
