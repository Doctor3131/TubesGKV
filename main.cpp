#include "Vector3.h"
#include "block.h"
#include "player.h"
#include "Renderer.h"
#include <iostream>



int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Block Grid");

    initBlocks();
    initOpenGL();

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
