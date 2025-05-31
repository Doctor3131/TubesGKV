#include "Renderer.h"
#include "renderChunks.h"
#include "loadimage/textureloader.h"

// Global texture ID - load once, use many times
GLuint stoneTextureID = 0;

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);  // PENTING: Enable texture 2D
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    
    // Load texture ONCE saat inisialisasi
    stoneTextureID = loadTexture("textures/stone.png");
    if (stoneTextureID == 0) {
        std::cerr << "Failed to load stone texture!" << std::endl;
    } else {
        std::cout << "Stone texture loaded successfully with ID: " << stoneTextureID << std::endl;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 800.0 / 600.0, 1.0, 100.0);
    
    // Set model-view
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(6.0, 6.0, 12.0,  // Camera position
              1.5, 1.5, 1.5,   // Look at point
              0.0, 1.0, 0.0);  // Up vector
    
    // Size and position for one block
    Vector3 size(3.0f, 3.0f, 3.0f);
    Vector3 pos(0.0f, 0.0f, 0.0f);
    
    // Draw cube with texture (gunakan texture yang sudah di-load)
    if (stoneTextureID != 0) {
        drawCube(pos, size, stoneTextureID);
    } else {
        // Fallback: draw white cube if no texture
        drawCube(pos, size, 0);
    }
    
    glutSwapBuffers();
}