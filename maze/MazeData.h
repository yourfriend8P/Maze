#pragma once

const int ROWS = 41;
const int COLS = 41;

// Declare these as extern
extern int dx[4];
extern int dy[4];
extern int maze[ROWS][COLS];
extern bool visited[ROWS][COLS];

//int dx[4] = { 0, 1, 0, -1 };
//int dy[4] = { -1, 0, 1, 0 };
//int maze[ROWS][COLS] = { 0 };
//bool visited[ROWS][COLS] = { false };

enum {
    WALL = 0,
    PATH = 1,
    START = 2,
    END = 3,
    VISITED = 4,
    SOLUTION = 5
};
