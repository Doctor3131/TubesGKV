#include "renderChunks.h"
#include "textureManager.h" 
#include "loadimage/textureloader.h"

#include "renderChunks.h"


Mat3d<std::shared_ptr<Block>> blockGrid(100, 100, 100);

// Helper function untuk set brightness
void setBrightness(float brightness) {
    glColor3f(brightness, brightness, brightness);
}

void initBlocks() {
    auto& texManager = TextureManager::getInstance();
    
    Vector3 size(1.0f, 1.0f, 1.0f);
    srand(static_cast<unsigned>(time(nullptr)));  // For noise offset

    float scale = 0.1f;  // Controls terrain "stretch"
    int baseHeight = 20;
    int amplitude = 10;

    for (size_t z = 0; z < 100; ++z) {
        for (size_t x = 0; x < 100; ++x) {
            // Fake Perlin: just using sin/cos noise
            float noise = std::sin(x * scale) + std::cos(z * scale);
            int height = static_cast<int>(baseHeight + noise * amplitude);

            for (size_t y = 0; y <= height && y < 100; ++y) {
                Vector3 pos(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));

                BlockType type;
                if (y == height) {
                    type = BlockType::Grass;
                } else if (y > height - 3) {
                    type = BlockType::Dirt;
                } else {
                    type = BlockType::Stone;
                }

                BlockTextureSet quartzTextures = texManager.getBlockTextures(type);
                blockGrid.at(z, y, x) = std::make_shared<Block>(type, pos, size, quartzTextures);
            }
        }
    }
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
}
