#include <GL/glut.h>
#include <cstdio>
#include <cstdlib>
#include <string>

// Game constants
const int WIDTH = 800;          // Width of the game window
const int HEIGHT = 600;         // Height of the game window
const int GRID_SIZE = 20;       // Size of each grid cell
const int GRID_WIDTH = WIDTH / GRID_SIZE;
const int GRID_HEIGHT = HEIGHT / GRID_SIZE;

// Snake variables
int snakeX[100], snakeY[100];    // Arrays to store snake segments
int snakeLength = 3;
int direction = 3;               // 1: Up, 2: Down, 3: Left, 4: Right

// Food variables
int foodX, foodY;

// Score and level variables
int score = 0;
int level = 1;

// Game state variables
bool isPlaying = true;
bool isPaused = false;
bool isGameOver = false;
bool hasWon = false;

// Initialize the game
void initGame() {
    // Initialize snake position
    snakeX[0] = GRID_WIDTH / 2;
    snakeY[0] = GRID_HEIGHT / 2;

    // Generate initial food position
    foodX = rand() % GRID_WIDTH;
    foodY = rand() % GRID_HEIGHT;

    // Reset score and level
    score = 0;
    level = 1;

    // Reset game state variables
    isPlaying = true;
    isPaused = false;
    isGameOver = false;
    hasWon = false;
}

// Draw the game scene
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (isPlaying) {
        // Draw snake
        glColor3f(0.0, 1.0, 0.0);    // Green color for the snake
        for (int i = 0; i < snakeLength; i++) {
            glRectf(snakeX[i] * GRID_SIZE, snakeY[i] * GRID_SIZE,
                    (snakeX[i] * GRID_SIZE) + GRID_SIZE,
                    (snakeY[i] * GRID_SIZE) + GRID_SIZE);
        }

        // Draw food
        glColor3f(1.0, 0.0, 0.0);    // Red color for the food
        glRectf(foodX * GRID_SIZE, foodY * GRID_SIZE,
                (foodX * GRID_SIZE) + GRID_SIZE,
                (foodY * GRID_SIZE) + GRID_SIZE);

        // Draw score and level
        glColor3f(1.0, 1.0, 1.0);    // White color for the score and level
        glRasterPos2f(10, HEIGHT - 30);
        std::string scoreString = "Score: " + std::to_string(score);
        for (char c : scoreString) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }

        glRasterPos2f(WIDTH - 90, HEIGHT - 30);
        std::string levelString = "Level: " + std::to_string(level);
        for (char c : levelString) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }

        if (isPaused) {
            // Draw "Paused" message
            glColor3f(1.0, 1.0, 1.0);
            glRasterPos2f((WIDTH / 2) - 30, HEIGHT / 2);
            std::string pausedString = "Paused";
            for (char c : pausedString) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
            }
        }
    }
    else {
        // Game over or won state

        if (isGameOver) {
            // Draw "Game Over" message
            glColor3f(1.0, 1.0, 1.0);
            glRasterPos2f((WIDTH / 2) - 40, HEIGHT / 2);
            std::string gameOverString = "Game Over";
            for (char c : gameOverString) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
            }
        }
        else if (hasWon) {
            // Draw "Congratulations" message
            glColor3f(1.0, 1.0, 1.0);
            glRasterPos2f((WIDTH / 2) - 60, HEIGHT / 2);
            std::string congratsString = "Congratulations! You Won!";
            for (char c : congratsString) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
            }
        }

        // Draw score and level
        glColor3f(1.0, 1.0, 1.0);    // White color for the score and level
        glRasterPos2f(10, HEIGHT - 30);
        std::string scoreString = "Score: " + std::to_string(score);
        for (char c : scoreString) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }

        glRasterPos2f(WIDTH - 90, HEIGHT - 30);
        std::string levelString = "Level: " + std::to_string(level);
        for (char c : levelString) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    glutSwapBuffers();
}

// Update the game state
void update(int value) {
    if (!isGameOver && isPlaying && !isPaused) {
        // Move the snake and update the game state
        for (int i = snakeLength - 1; i > 0; i--) {
            snakeX[i] = snakeX[i - 1];
            snakeY[i] = snakeY[i - 1];
        }

        // Update the snake's head position based on the direction
        switch (direction) {
            case 1:  // Up
                snakeY[0]++;
                break;
            case 2:  // Down
                snakeY[0]--;
                break;
            case 3:  // Left
                snakeX[0]--;
                break;
            case 4:  // Right
                snakeX[0]++;
                break;
        }

        // Wrap the snake around the grid boundaries
        if (snakeX[0] < 0)
            snakeX[0] = GRID_WIDTH - 1;
        else if (snakeX[0] >= GRID_WIDTH)
            snakeX[0] = 0;
        if (snakeY[0] < 0)
            snakeY[0] = GRID_HEIGHT - 1;
        else if (snakeY[0] >= GRID_HEIGHT)
            snakeY[0] = 0;

        // Check if the snake has collided with the food
        if (snakeX[0] == foodX && snakeY[0] == foodY) {
            // Increase the snake's length
            snakeLength++;

            // Generate new food position
            foodX = rand() % GRID_WIDTH;
foodY = rand() % GRID_HEIGHT;

            // Increase the score
            score++;

            // Check if level needs to be increased
            if (score % 10 == 0) {
                level++;
            }
        }

        // Check if the snake has collided with itself
        for (int i = 1; i < snakeLength; i++) {
            if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
                // Game over
                isGameOver = true;
                isPlaying = false;
                break;
            }
        }

        // Check if the player has reached 100 points
        if (score == 100) {
            hasWon = true;
            isPlaying = false;
        }
    }

    // Calculate the timer interval based on the current level *increase or decrease speed of snake(100) lower to increase speed*
    int timerInterval = 100 - (level * 10);

    // Update the game with the adjusted timer interval
    glutTimerFunc(timerInterval, update, 0);

    // Trigger redrawing of the game scene
    glutPostRedisplay();
}

// Handle keyboard input
void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': case 'W':  // Move up
            if (direction != 2)
                direction = 1;
            break;
        case 's': case 'S':  // Move down
            if (direction != 1)
                direction = 2;
            break;
        case 'a': case 'A':  // Move left
            if (direction != 4)
                direction = 3;
            break;
        case 'd': case 'D':  // Move right
            if (direction != 3)
                direction = 4;
            break;
        case ' ':  // Spacebar to toggle pause state
            isPaused = !isPaused;
            break;
    }
}

// Handle arrow key input
void handleSpecialKeypress(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:     // Move up
            if (direction != 2)
                direction = 1;
            break;
        case GLUT_KEY_DOWN:   // Move down
            if (direction != 1)
                direction = 2;
            break;
        case GLUT_KEY_LEFT:   // Move left
            if (direction != 4)
                direction = 3;
            break;
        case GLUT_KEY_RIGHT:  // Move right
            if (direction != 3)
                direction = 4;
            break;
    }
}

// Initialize the game and OpenGL settings
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    initGame();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Snake Game");

    glutDisplayFunc(drawScene);
    glutKeyboardFunc(handleKeypress);
    glutSpecialFunc(handleSpecialKeypress);

    init();

    glutTimerFunc(100, update, 0);

    glutMainLoop();
    return 0;
}
