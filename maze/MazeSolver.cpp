#include "MazeData.h"
#include <queue>
#include <map>
#include <iostream>

bool solveMazeBFS(int startX, int startY, int endX, int endY) {
    std::queue<std::pair<int, int>> q;
    bool visitedSolve[ROWS][COLS] = { false };
    std::map<std::pair<int, int>, std::pair<int, int>> parent;

    q.push({ startX, startY });
    visitedSolve[startY][startX] = true;

    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();

        if (x == endX && y == endY)
            break;

        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && ny >= 0 && nx < COLS && ny < ROWS &&
                maze[ny][nx] == PATH && !visitedSolve[ny][nx]) {
                q.push({ nx, ny });
                visitedSolve[ny][nx] = true;
                parent[{ nx, ny }] = { x, y };
            }
        }
    }

    auto p = std::make_pair(endX, endY);
    if (parent.find(p) == parent.end()) {
        std::cout << "No path found!\n";
        return false;
    }

    while (p != std::make_pair(startX, startY)) {
        maze[p.second][p.first] = SOLUTION;
        p = parent[p];
    }

    maze[startY][startX] = SOLUTION;
    return true;
}
