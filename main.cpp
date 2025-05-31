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
        return -1;  // Jika gagal, hentikan program
    }

    std::cout << "GLAD initialized successfully!" << std::endl;

    // Inisialisasi blok dan OpenGL
    initBlocks();
    initOpenGL();

    // Set fungsi display untuk merender
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
