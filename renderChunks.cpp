#include "renderChunks.h"
#include "loadimage/textureloader.h"

Mat3d<std::shared_ptr<Block>> blockGrid(3, 3, 3);

// Function to draw cube with texture
void drawCube(const Vector3& pos, const Vector3& size, GLuint textureID) {
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    
    if (textureID != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glColor3f(1.0f, 1.0f, 1.0f); // White color untuk texture normal
    } else {
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.8f, 0.8f, 0.8f); // Gray fallback color
    }
    
    glBegin(GL_QUADS);
    
    // Front face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size.x / 2, -size.y / 2, size.z / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size.x / 2, -size.y / 2, size.z / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size.x / 2, size.y / 2, size.z / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size.x / 2, size.y / 2, size.z / 2);
    
    // Back face
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-size.x / 2, -size.y / 2, -size.z / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-size.x / 2, size.y / 2, -size.z / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(size.x / 2, size.y / 2, -size.z / 2);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(size.x / 2, -size.y / 2, -size.z / 2);
    
    // Left face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size.x / 2, -size.y / 2, -size.z / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-size.x / 2, -size.y / 2, size.z / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-size.x / 2, size.y / 2, size.z / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size.x / 2, size.y / 2, -size.z / 2);
    
    // Right face
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size.x / 2, -size.y / 2, -size.z / 2);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(size.x / 2, -size.y / 2, size.z / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(size.x / 2, size.y / 2, size.z / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size.x / 2, size.y / 2, -size.z / 2);
    
    // Top face
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size.x / 2, size.y / 2, -size.z / 2);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size.x / 2, size.y / 2, size.z / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size.x / 2, size.y / 2, size.z / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size.x / 2, size.y / 2, -size.z / 2);
    
    // Bottom face
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-size.x / 2, -size.y / 2, -size.z / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(size.x / 2, -size.y / 2, -size.z / 2);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(size.x / 2, -size.y / 2, size.z / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-size.x / 2, -size.y / 2, size.z / 2);
    
    glEnd();
    glPopMatrix();
}

void initBlocks() {
    Vector3 size(1.0f, 1.0f, 1.0f);
    Vector3 pos(0.0f, 0.0f, 0.0f);
    BlockType type = BlockType::Stone;
    
    // Load texture for block (ini bisa dipindah ke texture manager nanti)
    GLuint textureID = loadTexture("textures/stone.png");
    
    // Initialize one block with texture
    auto block = std::make_shared<Block>(type, pos, size, textureID);
    blockGrid.at(0, 0, 0) = block;
}