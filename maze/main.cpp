#include "MazeData.h"
#include <GLFW/glfw3.h>
#include <iostream>

#define START_X 1
#define START_Y 1
#define END_X (COLS - 2)
#define END_Y (ROWS - 2)

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void generateMazeDFS(int x, int y); // Forward declaration
bool solveMazeBFS(int startX, int startY, int endX, int endY); // Forward declaration

bool mazeSolved = false;
bool keyPressed = false;

void drawCell(int row, int col, float r, float g, float b) {
    float cellSize = 0.0475f;
    float verticalPadding = 0.05f;
    float x = -1.0f + col * cellSize;
    float y = 1.0f - verticalPadding - row * cellSize;

    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + cellSize, y);
    glVertex2f(x + cellSize, y - cellSize);
    glVertex2f(x, y - cellSize);
    glEnd();
}

void drawMaze() {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            switch (maze[row][col]) {
            case WALL: drawCell(row, col, 0.0f, 0.0f, 0.0f); break;
            case PATH: drawCell(row, col, 1.0f, 1.0f, 1.0f); break;
            case START: drawCell(row, col, 1.0f, 0.0f, 0.0f); break;
            case END: drawCell(row, col, 0.0f, 1.0f, 0.0f); break;
            case VISITED: drawCell(row, col, 0.4f, 0.4f, 1.0f); break;
            case SOLUTION: drawCell(row, col, 1.0f, 1.0f, 0.0f); break;
            default: drawCell(row, col, 0.5f, 0.5f, 0.5f); break;
            }
        }
    }
}

int main() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* window = glfwCreateWindow(800, 900, "Maze", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize and generate maze
    for (int y = 0; y < ROWS; ++y)
        for (int x = 0; x < COLS; ++x)
            maze[y][x] = WALL;

    generateMazeDFS(START_X, START_Y);
    maze[START_Y][START_X] = PATH; // Important for solving
    maze[END_Y][END_X] = PATH;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Trigger solving only once when Enter key is pressed
        if (!mazeSolved && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
            if (!keyPressed) {
                keyPressed = true;
                mazeSolved = solveMazeBFS(START_X, START_Y, END_X, END_Y);
                // Now mark Start and End visually after solving
                maze[START_Y][START_X] = START;
                maze[END_Y][END_X] = END;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE) {
            keyPressed = false;
        }

        // Set up projection
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1, 1, -1, 1, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        drawMaze();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
