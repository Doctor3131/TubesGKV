#include "textureManager.h"
#include "loadimage/textureloader.h"
#include <iostream>

GLuint TextureManager::loadTexture(const std::string& path) {
    // Check if texture already loaded
    auto it = loadedTextures.find(path);
    if (it != loadedTextures.end()) {
        return it->second;
    }
    
    // Load new texture
    GLuint textureID = ::loadTexture(path.c_str()); // Call global loadTexture function
    loadedTextures[path] = textureID;
    return textureID;
}

void TextureManager::initializeBlockTextures() {
    // Load individual textures
    // Grass
    GLuint grass_top = loadTexture("textures/grass_top.png");
    GLuint grass_side = loadTexture("textures/grass_side.png");
    
    // Dirt
    GLuint dirt = loadTexture("textures/dirt.png");
    
    // Stone
    GLuint stone = loadTexture("textures/stone.png");
    
    // Wood
    GLuint wood_top = loadTexture("textures/wood_top.png");
    GLuint wood_side = loadTexture("textures/wood_side.png");
    
    // Crafting Table
    GLuint crafting_top = loadTexture("textures/craft_table_top.png");
    GLuint crafting_side = loadTexture("textures/craft_table_side.png");
    
    // Quartz
    GLuint quartz_top = loadTexture("textures/quartz_top.png");
    GLuint quartz_side = loadTexture("textures/quartz_side.png");

    // Di textureManager.cpp, update door texture setup:
    GLuint door_top = loadTexture("textures/door_top.png");
    GLuint door_bottom = loadTexture("textures/door_bottom.png");

  
    // Sun
    GLuint sun = loadTexture("textures/sun.png");


    // Load texture baru
     // TAMBAH TEXTURE BARU untuk objek kamu
    GLuint berry = loadTexture("textures/berry.png");
    GLuint bookshelf = loadTexture("textures/bookshelf.png");
    GLuint flower_leaves = loadTexture("textures/flower_leaves.png");
    GLuint ice = loadTexture("textures/ice.png");
    GLuint leaves = loadTexture("textures/leaves.png");
    GLuint stone_bricks = loadTexture("textures/stone_bricks.png");
    GLuint sand = loadTexture("textures/stone_bricks.png");

    GLuint bunga = loadTexture("textures/flower.png");

    // Simple blocks (same texture all sides) - GUNAKAN EMPLACE
    // TAMBAH MAPPING BARU (simple blocks - texture sama semua sisi)
    blockTextureMappings.emplace(BlockType::Berry, BlockTextureSet(berry));
    blockTextureMappings.emplace(BlockType::Bookshelf, BlockTextureSet(bookshelf));
    blockTextureMappings.emplace(BlockType::Flower_leaves, BlockTextureSet(flower_leaves));
    blockTextureMappings.emplace(BlockType::Ice, BlockTextureSet(ice));
    blockTextureMappings.emplace(BlockType::Leaves, BlockTextureSet(leaves));
    blockTextureMappings.emplace(BlockType::Stone_bricks, BlockTextureSet(stone_bricks));
    blockTextureMappings.emplace(BlockType::Flower, BlockTextureSet(bunga));


    blockTextureMappings.emplace(BlockType::Dirt, BlockTextureSet(dirt));
    blockTextureMappings.emplace(BlockType::Stone, BlockTextureSet(stone));
    blockTextureMappings.emplace(BlockType::Sand, BlockTextureSet(sand));
    
    // Complex blocks (different textures per side)
    blockTextureMappings.emplace(BlockType::Grass, BlockTextureSet(grass_side, grass_top, dirt));
    blockTextureMappings.emplace(BlockType::Wood, BlockTextureSet(wood_side, wood_top, wood_top));
    blockTextureMappings.emplace(BlockType::Crafting_table, BlockTextureSet(crafting_side, crafting_top, wood_side));
    blockTextureMappings.emplace(BlockType::Quartz, BlockTextureSet(quartz_side, quartz_top, quartz_top));
    
    
    // Special blocks
    blockTextureMappings.emplace(BlockType::Sun, BlockTextureSet(sun));

    // Special blocks - Door dengan texture yang benar
    blockTextureMappings.emplace(BlockType::Door, BlockTextureSet(door_bottom, door_bottom, door_bottom, door_bottom, door_top, door_bottom));
      
}

BlockTextureSet TextureManager::getBlockTextures(BlockType type) {
    auto it = blockTextureMappings.find(type);
    if (it != blockTextureMappings.end()) {
        return it->second;
    }
    // Fallback ke stone texture
    return BlockTextureSet(loadTexture("textures/stone.png"));
}

TextureManager& TextureManager::getInstance() {
    static TextureManager instance;
    return instance;
}