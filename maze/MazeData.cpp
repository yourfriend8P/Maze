#include "MazeData.h"

#define START_X 1
#define START_Y 1
#define END_X (COLS - 2)
#define END_Y (ROWS - 2)

// Directions: Up, Right, Down, Left
int dx[4] = { 0, 1, 0, -1 };
int dy[4] = { -1, 0, 1, 0 };

// Sample maze (0 = WALL, 1 = PATH)
//int maze[ROWS][COLS] = { 0 };
int maze[ROWS][COLS];


// Track visited cells for DFS/BFS
bool visited[ROWS][COLS] = { false };
