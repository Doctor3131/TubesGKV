#include "renderChunks.h"

Mat3d<std::shared_ptr<Block>> blockGrid(3, 3, 3);

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
    applyColor(type);
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    glutSolidCube(size.x);  // Assuming uniform cube
    glPopMatrix();
}

void initBlocks() {
    Vector3 size(1.0f, 1.0f, 1.0f);
    for (size_t z = 0; z < 3; ++z) {
        for (size_t y = 0; y < 3; ++y) {
            for (size_t x = 0; x < 3; ++x) {
                Vector3 pos(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
                BlockType type = static_cast<BlockType>((x + y + z) % 5);  // Cycle through types
                blockGrid.at(z, y, x) = std::make_shared<Block>(type, pos, size);
            }
        }
    }
}

