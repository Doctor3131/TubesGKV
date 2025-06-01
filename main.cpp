#include "Renderer.h"
#include <iostream>

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Interactive Scene");

    initOpenGL();
    initBlocks(); // Initialize the block grid

    setPlayerSpawnPosition();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(mouseMove);
    glutPassiveMotionFunc(mouseMove);
    
    // Set fungsi display untuk merender
    glutDisplayFunc(display);

    glutTimerFunc(16, updateScene, 0); // ~60 FPS update loop
    glutMainLoop();
    return 0;
}