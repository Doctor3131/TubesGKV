#include "Renderer.h"
#include "renderChunks.h"
#include "textureManager.h"
#include "loadimage/textureloader.h"

// Global texture ID - load once, use many times
GLuint stoneTextureID = 0;

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    
    // Initialize TextureManager (hapus loading stoneTextureID)
    TextureManager::getInstance().initializeBlockTextures();
}


// Renderer.cpp - Updated display function
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
    
    // Render all blocks in the grid
    auto& texManager = TextureManager::getInstance();
    
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            for (int z = 0; z < 3; z++) {
                auto block = blockGrid.at(x, y, z);
                if (block) {
                    // Get block textures and render
                    const BlockTextureSet& textures = block->getTextures();
                    drawCubeWithTextures(block->getPosition(), block->getSize(), textures, block.get());
                }
            }
        }
    }
    
    // Example: Manual rendering berbagai block types
    Vector3 size(1.0f, 1.0f, 1.0f);
    
    // Grass block
    Vector3 grassPos(-3.0f, 0.0f, 0.0f);
    BlockTextureSet grassTextures = texManager.getBlockTextures(BlockType::Grass);
    drawCubeWithTextures(grassPos, size, grassTextures);
    
    // Wood block
    Vector3 woodPos(-3.0f, 2.0f, 0.0f);
    BlockTextureSet woodTextures = texManager.getBlockTextures(BlockType::Wood);
    drawCubeWithTextures(woodPos, size, woodTextures);
    
    // Crafting table
    Vector3 craftPos(-3.0f, 4.0f, 0.0f);
    BlockTextureSet craftTextures = texManager.getBlockTextures(BlockType::Crafting_table);
    drawCubeWithTextures(craftPos, size, craftTextures);
    
    // Door (2 blocks high)
    Vector3 doorBottomPos(3.0f, 0.0f, 0.0f);
    Vector3 doorTopPos(3.0f, 1.0f, 0.0f);
    Vector3 doorSize(1.0f, 1.0f, 1.0f);
    
    BlockTextureSet doorTextures = texManager.getBlockTextures(BlockType::Door);
    Block doorBottom(BlockType::Door, doorBottomPos, doorSize, doorTextures, true);
    Block doorTop(BlockType::Door, doorTopPos, doorSize, doorTextures, false);
    
    drawCubeWithTextures(doorBottomPos, doorSize, doorTextures, &doorBottom);
    drawCubeWithTextures(doorTopPos, doorSize, doorTextures, &doorTop);
    
    // Sun block (only bottom face rendered)
    Vector3 sunPos(0.0f, 6.0f, 0.0f);
    BlockTextureSet sunTextures = texManager.getBlockTextures(BlockType::Sun);
    Block sunBlock(BlockType::Sun, sunPos, size, sunTextures);
    drawCubeWithTextures(sunPos, size, sunTextures, &sunBlock);
    
    glutSwapBuffers();
}