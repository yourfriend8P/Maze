#include "MazeData.h"
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>

bool isInBounds(int x, int y) {
    return x >= 0 && x < COLS && y >= 0 && y < ROWS;
}

void generateMazeDFS(int x, int y) {
    visited[y][x] = true;
    maze[y][x] = 1;

    int dirs[4] = { 0, 1, 2, 3 };

    // ✅ Create a random number generator
    static std::random_device rd;
    static std::mt19937 g(rd());

    // ✅ Shuffle directions using std::shuffle
    std::shuffle(std::begin(dirs), std::end(dirs), g);

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[dirs[i]] * 2;
        int ny = y + dy[dirs[i]] * 2;

        if (nx >= 0 && nx < COLS && ny >= 0 && ny < ROWS && !visited[ny][nx]) {
            maze[y + dy[dirs[i]]][x + dx[dirs[i]]] = 1; // remove wall between
            generateMazeDFS(nx, ny);
        }
    }
}

