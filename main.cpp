<<<<<<< HEAD
#include "vector3.h"
#include "block.h"
#include "player.h"
#include <iostream>

int main() {
    Player player;
    std::cout << "-- Testing Player move --\n";
    player.move(Vector3(1.0f, 0.0f, 0.0f));
    player.move(Vector3(0.0f, 1.0f, 0.0f));

    std::cout << "\n-- Testing Inventory & Place/Break --\n";
    // Simulate picking up a block by directly adding to inventory
    // We use breakBlock on a temporary block to add it
    Block* worldBlock = new Block(BlockType::Grass, Vector3(2, 2, 2), Vector3(1,1,1));
    player.breakBlock(worldBlock);

    // Try placing the block back at a new location
    bool placed = player.place(Vector3(5, 5, 5));
    std::cout << "Place returned: " << placed << "\n";

    std::cout << "\n-- Testing Inventory Slot Cycling --\n";
    player.selectNextSlot();
    player.selectNextSlot();
    player.selectPreviousSlot();

    std::cout << "\n-- All tests completed --\n";
    return 0;
}
=======
#include <glad/glad.h>      // ← WAJIB PALING AWAL sebelum file lain yang pakai OpenGL
#include <GL/freeglut.h>    // ← Setelah glad

#include "Vector3.h"
#include "block.h"
#include "player.h"
#include "Renderer.h"
#include <iostream>


int main(int argc, char** argv) {
    // Inisialisasi GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Block Grid");
    
    // Inisialisasi GLAD untuk memuat fungsi OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glutGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }
    std::cout << "GLAD initialized successfully!" << std::endl;
    
    // Inisialisasi blok dan OpenGL
    initBlocks();
    initOpenGL();
    
    // ===== TAMBAHKAN INI UNTUK TRANSPARANSI =====
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1f);
    // ==========================================
    
    // Set fungsi display untuk merender
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
>>>>>>> ren
