// ===== FIXED RENDERER.CPP - NO SYNTAX ERRORS =====
#include "Renderer.h"
#include "renderChunks.h"
#include "textureManager.h"
#include "loadimage/textureloader.h"
#include "Block.h"

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <chrono>

const float TOP_BRIGHTNESS = 1.0f;      // Top face paling terang (terkena matahari langsung)
const float FRONT_BRIGHTNESS = 0.8f;    // Front face terang
const float BACK_BRIGHTNESS = 0.8f;     // Back face terang  
const float LEFT_BRIGHTNESS = 0.6f;     // Left face agak gelap
const float RIGHT_BRIGHTNESS = 0.6f;    // Right face agak gelap
const float BOTTOM_BRIGHTNESS = 0.5f;   // Bottom face paling gelap

Player player;
auto& texManager = TextureManager::getInstance();

int frameCount = 0;
float fps = 0.0f;
auto lastTime = std::chrono::high_resolution_clock::now();

// Last mouse position
int lastMouseX = -1;
int lastMouseY = -1;

void keyboard(unsigned char key, int x, int y) {
    float moveSpeed = 0.1f;
    
    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    

    switch (key) {
        case 'w': player.moveForward(moveSpeed); break;
        case 's': player.moveBackward(moveSpeed); break;
        case 'a': player.moveLeft(moveSpeed); break;
        case 'd': player.moveRight(moveSpeed); break;
        case 'x': player.moveUp(moveSpeed); break; // Jump
        case 'c': player.moveDown(moveSpeed); break;
        case 'p': player.getClickedGroundCoordinate(player.lookDirection.x, player.lookDirection.y, windowWidth, windowHeight); break;
        case 27: exit(0); break;
        default: break;
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
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

// UPDATED: drawDoor dengan manual lighting
void drawDoor(const Vector3& pos, const Vector3& size, const BlockTextureSet& textures, bool isBottomPart) {
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    
    glEnable(GL_TEXTURE_2D);
    
    float doorThickness = 0.1f;
    float halfWidth = size.x / 2.0f;
    float halfHeight = size.y / 2.0f;
    
    GLuint doorTexture = isBottomPart ? 
        textures.getTexture(BlockFace::BOTTOM) : 
        textures.getTexture(BlockFace::TOP);
    
    glBindTexture(GL_TEXTURE_2D, doorTexture);
    
    // FRONT FACE (terang)
    setBrightness(FRONT_BRIGHTNESS);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfWidth, -halfHeight, doorThickness/2);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(halfWidth, -halfHeight, doorThickness/2);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(halfWidth, halfHeight, doorThickness/2);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfWidth, halfHeight, doorThickness/2);
    glEnd();
    
    // BACK FACE (terang)
    setBrightness(BACK_BRIGHTNESS);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfWidth, -halfHeight, -doorThickness/2);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfWidth, halfHeight, -doorThickness/2);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(halfWidth, halfHeight, -doorThickness/2);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(halfWidth, -halfHeight, -doorThickness/2);
    glEnd();
    
    // LEFT SIDE (agak gelap)
    setBrightness(LEFT_BRIGHTNESS);
    glBegin(GL_QUADS);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfWidth, -halfHeight, -doorThickness/2);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfWidth, -halfHeight, doorThickness/2);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfWidth, halfHeight, doorThickness/2);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfWidth, halfHeight, -doorThickness/2);
    glEnd();
    
    // RIGHT SIDE (agak gelap)
    setBrightness(RIGHT_BRIGHTNESS);
    glBegin(GL_QUADS);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(halfWidth, -halfHeight, -doorThickness/2);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(halfWidth, -halfHeight, doorThickness/2);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(halfWidth, halfHeight, doorThickness/2);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(halfWidth, halfHeight, -doorThickness/2);
    glEnd();
    
    // TOP (terang)
    setBrightness(TOP_BRIGHTNESS);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfWidth, halfHeight, -doorThickness/2);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfWidth, halfHeight, doorThickness/2);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(halfWidth, halfHeight, doorThickness/2);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(halfWidth, halfHeight, -doorThickness/2);
    glEnd();
    
    // BOTTOM (gelap)
    setBrightness(BOTTOM_BRIGHTNESS);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfWidth, -halfHeight, -doorThickness/2);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(halfWidth, -halfHeight, -doorThickness/2);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(halfWidth, -halfHeight, doorThickness/2);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfWidth, -halfHeight, doorThickness/2);
    glEnd();
    
    glPopMatrix();
}

// FIXED: initOpenGL() function
void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D); 
    glEnable(GL_CULL_FACE);

    // ===== TAMBAHKAN INI UNTUK TRANSPARANSI =====
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1f);
    // ==========================================

    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    glFrontFace(GL_CCW);    // Only this line, no glFrontFace(GL_CW);    
    // Setup lighting 
    setupLighting();
    
    // Initialize TextureManager
    TextureManager::getInstance().initializeBlockTextures();
}

void drawFrontTextureOnly(const Vector3& pos, const Vector3& size, GLuint frontTexture) {
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Set brightness untuk front texture
    setBrightness(FRONT_BRIGHTNESS);
    
    glBindTexture(GL_TEXTURE_2D, frontTexture);
    
    // Hanya render FRONT FACE dengan normal
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-size.x / 2, -size.y / 2, size.z / 2);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(size.x / 2, -size.y / 2, size.z / 2);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(size.x / 2, size.y / 2, size.z / 2);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-size.x / 2, size.y / 2, size.z / 2);
    glEnd();
    
    glPopMatrix();
}

// UPDATED: drawCubeWithTextures dengan manual lighting
void drawCubeWithTextures(const Vector3& pos, const Vector3& size, const BlockTextureSet& textures, const Block* block) {
    // Special case untuk door
    if (block && block->getType() == BlockType::Door) {
        bool isBottomPart = block->isBottomBlock();
        drawDoor(pos, size, textures, isBottomPart);
        return;
    }
    
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    
    // TOP FACE (paling terang - terkena matahari langsung)
    if (!block || block->shouldRenderFace(BlockFace::TOP)) {
        setBrightness(TOP_BRIGHTNESS);
        glBindTexture(GL_TEXTURE_2D, textures.getTexture(BlockFace::TOP));
        glBegin(GL_QUADS);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-size.x / 2, size.y / 2, -size.z / 2);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-size.x / 2, size.y / 2, size.z / 2);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(size.x / 2, size.y / 2, size.z / 2);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(size.x / 2, size.y / 2, -size.z / 2);
        glEnd();
    }
    
    // FRONT FACE (terang - pemain biasanya lihat dari sini)
    if (!block || block->shouldRenderFace(BlockFace::FRONT) || true) {
        setBrightness(FRONT_BRIGHTNESS);
        glBindTexture(GL_TEXTURE_2D, textures.getTexture(BlockFace::FRONT));
        glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, 1.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-size.x / 2, -size.y / 2, size.z / 2);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(size.x / 2, -size.y / 2, size.z / 2);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(size.x / 2, size.y / 2, size.z / 2);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-size.x / 2, size.y / 2, size.z / 2);
        glEnd();
    }
    
    // BACK FACE (terang - sama seperti front)
    if (!block || block->shouldRenderFace(BlockFace::BACK ) || true) {
        setBrightness(BACK_BRIGHTNESS);
        glBindTexture(GL_TEXTURE_2D, textures.getTexture(BlockFace::BACK));
        glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, -1.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-size.x / 2, -size.y / 2, -size.z / 2);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-size.x / 2, size.y / 2, -size.z / 2);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(size.x / 2, size.y / 2, -size.z / 2);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(size.x / 2, -size.y / 2, -size.z / 2);
        glEnd();
    }
    
    // LEFT FACE (agak gelap - side lighting)
    if (!block || block->shouldRenderFace(BlockFace::LEFT) || true) {
        setBrightness(LEFT_BRIGHTNESS);
        glBindTexture(GL_TEXTURE_2D, textures.getTexture(BlockFace::LEFT));
        glBegin(GL_QUADS);
            glNormal3f(-1.0f, 0.0f, 0.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-size.x / 2, -size.y / 2, -size.z / 2);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-size.x / 2, -size.y / 2, size.z / 2);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-size.x / 2, size.y / 2, size.z / 2);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-size.x / 2, size.y / 2, -size.z / 2);
        glEnd();
    }
    
    // RIGHT FACE (agak gelap - side lighting)
    if (!block || block->shouldRenderFace(BlockFace::RIGHT) || true) {
        setBrightness(RIGHT_BRIGHTNESS);
        glBindTexture(GL_TEXTURE_2D, textures.getTexture(BlockFace::RIGHT));
        glBegin(GL_QUADS);
            glNormal3f(1.0f, 0.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(size.x / 2, -size.y / 2, -size.z / 2);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(size.x / 2, -size.y / 2, size.z / 2);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(size.x / 2, size.y / 2, size.z / 2);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(size.x / 2, size.y / 2, -size.z / 2);
        glEnd();
    }
    
    // BOTTOM FACE (paling gelap - tidak terkena cahaya)
    if (!block || block->shouldRenderFace(BlockFace::BOTTOM)) {
        setBrightness(BOTTOM_BRIGHTNESS);
        glBindTexture(GL_TEXTURE_2D, textures.getTexture(BlockFace::BOTTOM));
        glBegin(GL_QUADS);
            glNormal3f(0.0f, -1.0f, 0.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-size.x / 2, -size.y / 2, -size.z / 2);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(size.x / 2, -size.y / 2, -size.z / 2);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(size.x / 2, -size.y / 2, size.z / 2);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-size.x / 2, -size.y / 2, size.z / 2);
        glEnd();
    }
    
    glPopMatrix();
}

// FIXED: display() function dengan lighting yang lebih baik
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
                const Block* rawPtr = block.get();
                if (block) {
                    // Temp Solution ideally we should do face culling
                    bool covered = false;

                    if (!covered) {
                        drawCubeWithTextures(block->getPosition(), block->getSize(), block->getTextures(), rawPtr);
                    }
                }
            }
        }
    }
    
    // Update light position setiap frame dengan posisi yang lebih optimal
    GLfloat lightPosition[] = {2.0f, 15.0f, 8.0f, 1.0f};  // Posisi cahaya lebih ke depan
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    
    // TAMBAHAN: Set warna global lebih terang
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    
        // Draw the player
    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(player.getPosition().x, player.getPosition().y, player.getPosition().z);
    // glutSolidCube(1.0f);
    glPopMatrix();
    glutSwapBuffers();
}

bool justWarped = false;
int centerX, centerY;
float yaw = 0.0f, pitch = 0.0f;

void mouseMove(int x, int y) {
    if (justWarped) {
        justWarped = false;
        return;
    }

    int xoffset = x - lastMouseX;
    int yoffset = lastMouseY - y;  // reversed since y-coordinates go downwards

    lastMouseX = centerX;
    lastMouseY = centerY;

    float sensitivity = 0.1f;  // tweak to control speed
    float xoffsetf = xoffset * sensitivity;
    float yoffsetf = yoffset * sensitivity;

    player.look(xoffsetf, yoffsetf);

    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    centerX = windowWidth / 2;
    centerY = windowHeight / 2;

    glutWarpPointer(centerX, centerY);
    justWarped = true;
    glutPostRedisplay();  // request redraw

}