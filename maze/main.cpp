#define STB_EASY_FONT_IMPLEMENTATION
#include "stb_easy_font.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include "MazeData.h"

#define START_X 1
#define START_Y 1
#define END_X (COLS - 2)
#define END_Y (ROWS - 2)

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void generateMazeDFS(int x, int y); // your function
bool solveMazeBFS(int startX, int startY, int endX, int endY); // your function

bool bfsSolved = false;

void renderText(float x, float y, const char* text) {
    char buffer[99999]; // large buffer for text vertices
    int num_quads = stb_easy_font_print(x, y, (char*)text, NULL, buffer, sizeof(buffer));

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 16, buffer);
    glDrawArrays(GL_QUADS, 0, num_quads * 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPixelZoom(1.0f, -1.0f);
    glRasterPos2f(x, -y);  // Invert Y position

}

void drawCell(int row, int col, float r, float g, float b) {
    float cellSize = 0.0475f; // fits nicely into [-1, 1] range
    // Add vertical and horizontal padding
    float verticalPadding = 0.05f; // Adjust this to increase/decrease top-bottom space
    float horizontalPadding = 0.0f; // Optional: Add if you want space on sides too

    float x = -1.0f + horizontalPadding + col * cellSize;
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

    // Initialize maze
    for (int y = 0; y < ROWS; ++y)
        for (int x = 0; x < COLS; ++x)
            maze[y][x] = WALL;

    generateMazeDFS(1, 1);
    //solveMazeBFS(START_X, START_Y, END_X, END_Y);
    maze[START_Y][START_X] = END;
    maze[END_Y][END_X] = START;
    solveMazeBFS(START_X, START_Y, END_X, END_Y);

    // MAIN LOOP
    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

       /* if (!bfsSolved && glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
            solveMazeBFS(START_X, START_Y, END_X, END_Y);
            bfsSolved = true;
        }*/

        // Default projection for maze
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1, 1, -1, 1, -1, 1);  // ← Use your original projection
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        drawMaze();

        // Switch projection for text only
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, 800, 800, 0, -1, 1); // ← Top-left origin for text
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glColor3f(1.0f, 0.0f, 0.0f); // black text
        renderText(20, 25, "Press 'X' for solution");

        glPopMatrix(); // restore modelview
        glMatrixMode(GL_PROJECTION);
        glPopMatrix(); // restore projection
        glMatrixMode(GL_MODELVIEW); // restore state

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
