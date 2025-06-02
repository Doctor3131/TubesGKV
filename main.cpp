#include <windows.h>

#include "Renderer.h"
#include <iostream>


void ClipCursorToWindow() {
    HWND hwnd = GetActiveWindow();  // Get the current GLUT window handle

    if (hwnd) {
        RECT rect;
        GetClientRect(hwnd, &rect);                // Get client size
        POINT ul = { rect.left, rect.top };        // Upper-left
        POINT lr = { rect.right, rect.bottom };    // Lower-right

        ClientToScreen(hwnd, &ul);  // Convert to screen coords
        ClientToScreen(hwnd, &lr);

        RECT screenRect = { ul.x, ul.y, lr.x, lr.y };
        ClipCursor(&screenRect);
    }
}

void ReleaseCursor() {
    ClipCursor(NULL);
}

void idle() {
    HWND hwnd = GetActiveWindow();

    if (GetForegroundWindow() == hwnd && hwnd != nullptr && IsIconic(hwnd) == FALSE) {
        // Clip the cursor to the client area
        ClipCursorToWindow();
    } else {
        // If window is not focused or minimized, release cursor
        ReleaseCursor();
    }

    glutPostRedisplay();
}


void cleanup() {
    ReleaseCursor();
    ShowCursor(TRUE);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Interactive Scene");
    glutSetCursor(GLUT_CURSOR_NONE);

    initOpenGL();
    initBlocks(); // Initialize the block grid

    setPlayerSpawnPosition();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    // glutMotionFunc(mouseMove);
    glutPassiveMotionFunc(mouseMove);
    glutIdleFunc(idle);
    
    ShowCursor(FALSE);     // Hide cursor

    // Warp to center initially
    // justWarped = true;
    // glutWarpPointer(centerX, centerY);

    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    centerX = windowWidth / 2;
    centerY = windowHeight / 2;

    justWarped = true;
    glutWarpPointer(centerX, centerY);

    atexit(cleanup);       // Make sure cursor is released on exit

    glutTimerFunc(16, updateScene, 0); // ~60 FPS update loop
    glutMainLoop();
    return 0;
}