#include "Renderer.h"
#include "renderChunks.h"
#include "textureManager.h"
#include "loadimage/textureloader.h"

// Global texture ID - load once, use many times
GLuint stoneTextureID = 0;

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    
    // Initialize TextureManager (hapus loading stoneTextureID)
    TextureManager::getInstance().initializeBlockTextures();
}


// Renderer.cpp - Updated display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 800.0 / 600.0, 1.0, 100.0);
    
    // Set model-view
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(6.0, 6.0, 12.0,
              1.5, 1.5, 1.5,
              0.0, 1.0, 0.0);
    
    // Langsung panggil initBlocks() yang udah ada rendering di dalamnya
    initBlocks();
   
    
    glutSwapBuffers();
}