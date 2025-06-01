// ===== FIXED RENDERER.CPP - NO SYNTAX ERRORS =====
#include "Renderer.h"
#include "renderChunks.h"
#include "textureManager.h"
#include "loadimage/textureloader.h"

void setupLighting() {
    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // FIXED: Minecraft-style lighting dengan nilai yang lebih terang
    GLfloat lightAmbient[] = {0.6f, 0.6f, 0.6f, 1.0f};     // Ambient lebih terang
    GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};     // Diffuse maksimal untuk terang
    GLfloat lightSpecular[] = {0.3f, 0.3f, 0.3f, 1.0f};    // Specular dikurangi
    
    // Position light dari atas tengah (seperti matahari di Minecraft)
    GLfloat lightPosition[] = {0.0f, 15.0f, 5.0f, 1.0f};   // Sedikit ke depan dan lebih tinggi
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    
    // FIXED: Material properties yang lebih terang seperti Minecraft
    GLfloat materialAmbient[] = {0.8f, 0.8f, 0.8f, 1.0f};   // Ambient material lebih terang
    GLfloat materialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};   // Diffuse maksimal
    GLfloat materialSpecular[] = {0.1f, 0.1f, 0.1f, 1.0f};  // Specular minimal (Minecraft tidak glossy)
    GLfloat materialShininess = 1.0f;                        // Shininess minimal
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialShininess);
    
    // Enable automatic normal calculation
    glEnable(GL_NORMALIZE);
    
    // TAMBAHAN: Color material untuk mempertahankan warna texture
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

// FIXED: initOpenGL() function
void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.4f, 0.6f, 0.9f, 1.0f);  // Sky color seperti Minecraft
    
    // Setup lighting 
    setupLighting();
    
    // Initialize TextureManager
    TextureManager::getInstance().initializeBlockTextures();
}

// FIXED: display() function dengan lighting yang lebih baik
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
    
    // Update light position setiap frame dengan posisi yang lebih optimal
    GLfloat lightPosition[] = {2.0f, 15.0f, 8.0f, 1.0f};  // Posisi cahaya lebih ke depan
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    
    // TAMBAHAN: Set warna global lebih terang
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    
    // Render blocks
    initBlocks();
    
    glutSwapBuffers();
}