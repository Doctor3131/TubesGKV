#pragma once
// #include <glad/glad.h>
#include "Vector3.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include "Block.h"

// Texture Manager untuk handle loading dan mapping
class TextureManager {
private:
    std::unordered_map<std::string, GLuint> loadedTextures;
    std::unordered_map<BlockType, BlockTextureSet> blockTextureMappings;
   
public:
    GLuint loadTexture(const std::string& path);
    void initializeBlockTextures();
    BlockTextureSet getBlockTextures(BlockType type);
    
    static TextureManager& getInstance();
};