#include "renderChunks.h"
#include "textureManager.h" 
#include "loadimage/textureloader.h"

Mat3d<std::shared_ptr<Block>> blockGrid(3, 3, 3);

// FIXED: Door rendering function - bikin door tipis kayak Minecraft
void drawDoor(const Vector3& pos, const Vector3& size, const BlockTextureSet& textures, bool isBottomPart) {
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Door thickness (tipis kayak Minecraft)
    float doorThickness = 0.1f;
    float halfWidth = size.x / 2.0f;
    float halfHeight = size.y / 2.0f;
    
    // Pilih texture berdasarkan bottom/top part
    GLuint doorTexture = isBottomPart ? 
        textures.getTexture(BlockFace::BOTTOM) : 
        textures.getTexture(BlockFace::TOP);
    
    glBindTexture(GL_TEXTURE_2D, doorTexture);
    
    // FRONT FACE (main door face)
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfWidth, -halfHeight, doorThickness/2);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(halfWidth, -halfHeight, doorThickness/2);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(halfWidth, halfHeight, doorThickness/2);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfWidth, halfHeight, doorThickness/2);
    glEnd();
    
    // BACK FACE 
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfWidth, -halfHeight, -doorThickness/2);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfWidth, halfHeight, -doorThickness/2);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(halfWidth, halfHeight, -doorThickness/2);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(halfWidth, -halfHeight, -doorThickness/2);
    glEnd();
    
    // SIDES (tipis)
    // Left side
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfWidth, -halfHeight, -doorThickness/2);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfWidth, -halfHeight, doorThickness/2);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfWidth, halfHeight, doorThickness/2);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfWidth, halfHeight, -doorThickness/2);
    glEnd();
    
    // Right side
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(halfWidth, -halfHeight, -doorThickness/2);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(halfWidth, -halfHeight, doorThickness/2);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(halfWidth, halfHeight, doorThickness/2);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(halfWidth, halfHeight, -doorThickness/2);
    glEnd();
    
    // TOP (kalau perlu)
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfWidth, halfHeight, -doorThickness/2);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfWidth, halfHeight, doorThickness/2);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(halfWidth, halfHeight, doorThickness/2);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(halfWidth, halfHeight, -doorThickness/2);
    glEnd();
    
    // BOTTOM (kalau perlu)
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfWidth, -halfHeight, -doorThickness/2);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(halfWidth, -halfHeight, -doorThickness/2);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(halfWidth, -halfHeight, doorThickness/2);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfWidth, -halfHeight, doorThickness/2);
    glEnd();
    
    glPopMatrix();
}




// Update drawCubeWithTextures untuk handle door
void drawCubeWithTextures(const Vector3& pos, const Vector3& size, const BlockTextureSet& textures, const Block* block) {
    // Special case untuk door
    if (block && block->getType() == BlockType::Door) {
        bool isBottomPart = block->isBottomBlock();
        drawDoor(pos, size, textures, isBottomPart);
        return;
    }
    
    // Regular cube rendering untuk block lain
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // FRONT FACE
    if (!block || block->shouldRenderFace(BlockFace::FRONT)) {
        glBindTexture(GL_TEXTURE_2D, textures.getTexture(BlockFace::FRONT));
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-size.x / 2, -size.y / 2, size.z / 2);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(size.x / 2, -size.y / 2, size.z / 2);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(size.x / 2, size.y / 2, size.z / 2);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-size.x / 2, size.y / 2, size.z / 2);
        glEnd();
    }
    
    // BACK FACE
    if (!block || block->shouldRenderFace(BlockFace::BACK)) {
        glBindTexture(GL_TEXTURE_2D, textures.getTexture(BlockFace::BACK));
        glBegin(GL_QUADS);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-size.x / 2, -size.y / 2, -size.z / 2);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-size.x / 2, size.y / 2, -size.z / 2);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(size.x / 2, size.y / 2, -size.z / 2);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(size.x / 2, -size.y / 2, -size.z / 2);
        glEnd();
    }
    
    // LEFT FACE
    if (!block || block->shouldRenderFace(BlockFace::LEFT)) {
        glBindTexture(GL_TEXTURE_2D, textures.getTexture(BlockFace::LEFT));
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-size.x / 2, -size.y / 2, -size.z / 2);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-size.x / 2, -size.y / 2, size.z / 2);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-size.x / 2, size.y / 2, size.z / 2);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-size.x / 2, size.y / 2, -size.z / 2);
        glEnd();
    }
    
    // RIGHT FACE
    if (!block || block->shouldRenderFace(BlockFace::RIGHT)) {
        glBindTexture(GL_TEXTURE_2D, textures.getTexture(BlockFace::RIGHT));
        glBegin(GL_QUADS);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(size.x / 2, -size.y / 2, -size.z / 2);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(size.x / 2, -size.y / 2, size.z / 2);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(size.x / 2, size.y / 2, size.z / 2);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(size.x / 2, size.y / 2, -size.z / 2);
        glEnd();
    }
    
    // TOP FACE
    if (!block || block->shouldRenderFace(BlockFace::TOP)) {
        glBindTexture(GL_TEXTURE_2D, textures.getTexture(BlockFace::TOP));
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-size.x / 2, size.y / 2, -size.z / 2);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-size.x / 2, size.y / 2, size.z / 2);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(size.x / 2, size.y / 2, size.z / 2);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(size.x / 2, size.y / 2, -size.z / 2);
        glEnd();
    }
    
    // BOTTOM FACE
    if (!block || block->shouldRenderFace(BlockFace::BOTTOM)) {
        glBindTexture(GL_TEXTURE_2D, textures.getTexture(BlockFace::BOTTOM));
        glBegin(GL_QUADS);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-size.x / 2, -size.y / 2, -size.z / 2);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(size.x / 2, -size.y / 2, -size.z / 2);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(size.x / 2, -size.y / 2, size.z / 2);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-size.x / 2, -size.y / 2, size.z / 2);
        glEnd();
    }
    
    glPopMatrix();
}



// Legacy function untuk backward compatibility
void drawCube(const Vector3& pos, const Vector3& size, GLuint textureID) {
    BlockTextureSet simpleTexture(textureID);
    drawCubeWithTextures(pos, size, simpleTexture);
}

// Function untuk place door (2 blocks)
void placeDoor(const Vector3& bottomPos, BlockType doorType) {
    Vector3 size(1.0f, 1.0f, 1.0f);
    Vector3 topPos = bottomPos;
    topPos.y += 1.0f;
    
    auto& texManager = TextureManager::getInstance();
    BlockTextureSet doorTextures = texManager.getBlockTextures(doorType);
    
    // Bottom part
    auto bottomDoor = std::make_shared<Block>(doorType, bottomPos, size, doorTextures, true);
    // Top part  
    auto topDoor = std::make_shared<Block>(doorType, topPos, size, doorTextures, false);
    
    // Add to grid (assuming we can place at these positions)
    // blockGrid.at(x, y, z) = bottomDoor;
    // blockGrid.at(x, y+1, z) = topDoor;
}

void initBlocks() {
    // Initialize texture manager
    auto& texManager = TextureManager::getInstance();
    texManager.initializeBlockTextures();

    Vector3 size(1.0f, 1.0f, 1.0f);
    
    // Example: Create different block types
    // Grass block
    Vector3 grassPos(0.0f, 0.0f, 0.0f);
    BlockTextureSet grassTextures = texManager.getBlockTextures(BlockType::Grass);
    auto grassBlock = std::make_shared<Block>(BlockType::Grass, grassPos, size, grassTextures);
    blockGrid.at(0, 0, 0) = grassBlock;
    
    // Wood block
    Vector3 woodPos(2.0f, 0.0f, 0.0f);
    BlockTextureSet woodTextures = texManager.getBlockTextures(BlockType::Wood);
    auto woodBlock = std::make_shared<Block>(BlockType::Wood, woodPos, size, woodTextures);
    blockGrid.at(1, 0, 0) = woodBlock;
    
    // Crafting table
    Vector3 craftPos(4.0f, 0.0f, 0.0f);
    BlockTextureSet craftTextures = texManager.getBlockTextures(BlockType::Crafting_table);
    auto craftBlock = std::make_shared<Block>(BlockType::Crafting_table, craftPos, size, craftTextures);
    blockGrid.at(2, 0, 0) = craftBlock;
}