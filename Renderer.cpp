#include "Renderer.h"

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 800.0 / 600.0, 1.0, 100.0); // FOV, aspect ratio, near, far

    // Set model-view
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(6.0, 6.0, 12.0,  // Camera position
              1.5, 1.5, 1.5,   // Look-at point (center of 3x3x3 grid)
              0.0, 1.0, 0.0);  // Up direction

    for (size_t z = 0; z < 3; ++z) {
        for (size_t y = 0; y < 3; ++y) {
            for (size_t x = 0; x < 3; ++x) {
                auto block = blockGrid.at(z, y, x);
                if (block) {
                    drawCube(block->getPosition(), block->getSize(), block->getType());
                }
            }
        }
    }

    glutSwapBuffers();
}
