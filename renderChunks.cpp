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
    auto& texManager = TextureManager::getInstance();
    Vector3 size(1.0f, 1.0f, 1.0f);
    
    // Grass block - langsung render
    Vector3 grassPos(-3.0f, 0.0f, 0.0f);
    BlockTextureSet grassTextures = texManager.getBlockTextures(BlockType::Grass);
    drawCubeWithTextures(grassPos, size, grassTextures);
    
    // Wood block - langsung render
    Vector3 woodPos(-3.0f, 2.0f, 0.0f);
    BlockTextureSet woodTextures = texManager.getBlockTextures(BlockType::Wood);
    drawCubeWithTextures(woodPos, size, woodTextures);
    
    // Crafting table - langsung render
    Vector3 craftPos(-3.0f, 4.0f, 0.0f);
    BlockTextureSet craftTextures = texManager.getBlockTextures(BlockType::Crafting_table);
    drawCubeWithTextures(craftPos, size, craftTextures);
    
    // Door - langsung render
    Vector3 doorBottomPos(3.0f, 0.0f, 0.0f);
    Vector3 doorTopPos(3.0f, 1.0f, 0.0f);
    BlockTextureSet doorTextures = texManager.getBlockTextures(BlockType::Door);
    
    Block doorBottom(BlockType::Door, doorBottomPos, size, doorTextures, true);
    Block doorTop(BlockType::Door, doorTopPos, size, doorTextures, false);
    drawCubeWithTextures(doorBottomPos, size, doorTextures, &doorBottom);
    drawCubeWithTextures(doorTopPos, size, doorTextures, &doorTop);
    
    // Sun block - langsung render
    Vector3 sunPos(0.0f, 6.0f, 0.0f);
    BlockTextureSet sunTextures = texManager.getBlockTextures(BlockType::Sun);
    Block sunBlock(BlockType::Sun, sunPos, size, sunTextures);
    drawCubeWithTextures(sunPos, size, sunTextures, &sunBlock);

     // OBJEK BARU - di samping pintu
    float doorX = 3.0f; // posisi X pintu
    
    // Berry - di kiri pintu
    Vector3 berryPos(doorX - 2.0f, 0.0f, 0.0f);
    BlockTextureSet berryTextures = texManager.getBlockTextures(BlockType::Berry);
    drawCubeWithTextures(berryPos, size, berryTextures);
    
    // Bookshelf - di kanan pintu
    Vector3 bookshelfPos(doorX + 2.0f, 0.0f, 0.0f);
    BlockTextureSet bookshelfTextures = texManager.getBlockTextures(BlockType::Bookshelf);
    drawCubeWithTextures(bookshelfPos, size, bookshelfTextures);
    
    // Dirt - di belakang pintu
    Vector3 dirtPos(doorX, 0.0f, -2.0f);
    BlockTextureSet dirtTextures = texManager.getBlockTextures(BlockType::Dirt);
    drawCubeWithTextures(dirtPos, size, dirtTextures);
    
    // Flower leaves - di depan pintu
    Vector3 flowerPos(doorX, 0.0f, 2.0f);
    BlockTextureSet flowerTextures = texManager.getBlockTextures(BlockType::Flower_leaves);
    drawCubeWithTextures(flowerPos, size, flowerTextures);
    
    // Ice - di atas kiri pintu
    Vector3 icePos(doorX - 1.0f, 2.0f, 0.0f);
    BlockTextureSet iceTextures = texManager.getBlockTextures(BlockType::Ice);
    drawCubeWithTextures(icePos, size, iceTextures);
    
    // Leaves - di atas kanan pintu
    Vector3 leavesPos(doorX + 1.0f, 2.0f, 0.0f);
    BlockTextureSet leavesTextures = texManager.getBlockTextures(BlockType::Leaves);
    drawCubeWithTextures(leavesPos, size, leavesTextures);
    
    // Sand - di bawah kiri
    Vector3 sandPos(doorX - 1.0f, -1.0f, 0.0f);
    BlockTextureSet sandTextures = texManager.getBlockTextures(BlockType::Sand);
    drawCubeWithTextures(sandPos, size, sandTextures);
    
    // Stone brick - di bawah kanan
    Vector3 stoneBrickPos(doorX + 1.0f, -1.0f, 0.0f);
    BlockTextureSet stoneBrickTextures = texManager.getBlockTextures(BlockType::Stone_bricks);
    drawCubeWithTextures(stoneBrickPos, size, stoneBrickTextures);
}