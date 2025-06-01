#include "Renderer.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>

int frameCount = 0;
float fps = 0.0f;
auto lastTime = std::chrono::high_resolution_clock::now();

// Player player = new Player();

Player player;

// Last mouse position
int lastMouseX = -1;
int lastMouseY = -1;

void updateFPS() {
    frameCount++;

    auto currentTime = std::chrono::high_resolution_clock::now();
    float elapsedSeconds = std::chrono::duration<float>(currentTime - lastTime).count();

    if (elapsedSeconds >= 1.0f) {
        fps = frameCount / elapsedSeconds;

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << "FPS: " << fps;

        glutSetWindowTitle(oss.str().c_str());

        frameCount = 0;
        lastTime = currentTime;
    }
}

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);    // Only this line, no glFrontFace(GL_CW);
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
}

void keyboard(unsigned char key, int x, int y) {
    float moveSpeed = 0.1f;
    switch (key) {
        case 'w': player.moveForward(moveSpeed); break;
        case 's': player.moveBackward(moveSpeed); break;
        case 'a': player.moveLeft(moveSpeed); break;
        case 'd': player.moveRight(moveSpeed); break;
        case 'x': player.moveUp(moveSpeed); break; // Jump
        case 'c': player.moveDown(moveSpeed); break;
        case 'p': player.place(player.getPosition() - Vector3(player.lookDirection * 2.0f)); break;
        case 27: exit(0); break;
        default: break;
    }
}

void drawText(float x, float y, const char* text) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);  // Adjust to your window size

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Normalize lookDirection to ensure correct camera target
    Vector3 lookDirNormalized = player.lookDirection.normalized();

    // Set camera position: behind and slightly above player
    Vector3 eye = player.getPosition() + Vector3(-5.0f, 3.0f, -5.0f);

    // Camera looks towards player look direction
    Vector3 center = eye + lookDirNormalized;

    // Up vector remains world up
    Vector3 up(0.0f, 1.0f, 0.0f);

    gluLookAt(eye.x, eye.y, eye.z,
            center.x, center.y, center.z,
            up.x, up.y, up.z);

    for (size_t z = 0; z < std::min(blockGrid.depth(), size_t(50)); ++z) {
        for (size_t y = 0; y < std::min(blockGrid.rows(), size_t(50)); ++y) {
            for (size_t x = 0; x < std::min(blockGrid.cols(), size_t(50)); ++x) {
                auto block = blockGrid.at(z, y, x);
                if (block) {
                    drawCube(block->getPosition(), block->getSize(), block->getType());
                }
            }
        }
    }
    updateFPS();

    // Before drawing text
    glDisable(GL_DEPTH_TEST);

    glColor3f(1.0f, 1.0f, 1.0f);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << "FPS: " << fps;
    drawText(10.0f, 20.0f, oss.str().c_str());

    glEnable(GL_DEPTH_TEST);

    // Draw the player
    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(player.getPosition().x, player.getPosition().y, player.getPosition().z);
    // glutSolidCube(1.0f);
    glPopMatrix();
    glutSwapBuffers();
}

void setPlayerSpawnPosition() {
    int spawnX = 50;
    int spawnZ = 50;
    int maxY = 99;

    float spawnY = 1.0f; // default ground level if no block found
    for (int y = maxY; y >= 0; --y) {
        auto block = blockGrid.at(spawnZ, y, spawnX);
        if (block) {
            spawnY = static_cast<float>(y) + 2.0f;  // spawn just above block
            break;
        }
    }

    player.setPosition(Vector3(spawnX, spawnY, spawnZ));
}

void updateScene(int value) {
    player.update(1.0f / 60.0f);
    glutTimerFunc(16, updateScene, 0);
}

void mouseMove(int x, int y) {
    if (lastMouseX != -1 && lastMouseY != -1) {
        float deltaX = x - lastMouseX;
        float deltaY = y - lastMouseY;
        player.look(deltaX, deltaY);
    }
    lastMouseX = x;
    lastMouseY = y;
}