#include "renderChunks.h"

Mat3d<std::shared_ptr<Block>> blockGrid(100, 100, 100);

void applyColor(BlockType type) {
    switch (type) {
        case BlockType::Grass: glColor3f(0.0f, 1.0f, 0.0f); break;
        case BlockType::Dirt:  glColor3f(0.5f, 0.25f, 0.0f); break;
        case BlockType::Stone: glColor3f(0.5f, 0.5f, 0.5f); break;
        case BlockType::Wood:  glColor3f(0.55f, 0.27f, 0.07f); break;
        case BlockType::Sand:  glColor3f(0.96f, 0.87f, 0.7f); break;
    }
}

void drawCube(const Vector3& pos, const Vector3& size, BlockType type) {
    // std::cout << "Drawing block at: " << pos.x << ", " << pos.y << ", " << pos.z << "\n";
    applyColor(type);
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    glutSolidCube(size.x);  // Assuming uniform cube
    glPopMatrix();
}

void initBlocks() {
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

                blockGrid.at(z, y, x) = std::make_shared<Block>(type, pos, size);
            }
        }
    }
}


