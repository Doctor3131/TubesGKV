// In your main .cpp or player.cpp
#include "Player.h"
#include <cmath>

Player::Player() : position(0.0f, 0.0f, 0.0f), velocity(0.0f, 0.0f, 0.0f), lookDirection(0.0f, 0.0f, -1.0f) {}

void Player::moveForward(float speed) {
    Vector3 forward(lookDirection.x, 0.0f, lookDirection.z);
    if (forward.lengthSquared() > 0.0f) {
        forward = forward.normalized();
        position += forward * speed;
        glutPostRedisplay();
    }
}

void Player::moveBackward(float speed) {
    Vector3 forward(lookDirection.x, 0.0f, lookDirection.z);
    if (forward.lengthSquared() > 0.0f) {
        forward = forward.normalized();
        position -= forward * speed;
        glutPostRedisplay();
    }
}

void Player::setPosition(const Vector3& newPos) {
    position = newPos;
}

void Player::moveLeft(float speed) {
    Vector3 forward(lookDirection.x, 0.0f, lookDirection.z);
    Vector3 right;
    if (forward.lengthSquared() > 0.0f) {
        forward = forward.normalized();
        right = Vector3(0.0f, 1.0f, 0.0f).cross(forward).normalized(); // Changed order for correct left direction
        position += right * speed;
        glutPostRedisplay();
    }
}

void Player::moveRight(float speed) {
    Vector3 forward(lookDirection.x, 0.0f, lookDirection.z);
    Vector3 right;
    if (forward.lengthSquared() > 0.0f) {
        forward = forward.normalized();
        right = Vector3(0.0f, 1.0f, 0.0f).cross(forward).normalized();
        position -= right * speed;
    }
    glutPostRedisplay();
}

void Player::moveUp(float speed) {
    position.y += speed; 
    glutPostRedisplay();
}

void Player::moveDown(float speed) {
    position.y -= speed;
    glutPostRedisplay();
}

// void Player::place(const Vector3& target) {
//     Vector3 blockSize(1.0f, 1.0f, 1.0f);
//     BlockType blockType = BlockType::Wood;
//     auto& texManager = TextureManager::getInstance();

//     int bx = static_cast<int>(round(target.x));
//     int by = static_cast<int>(round(target.y));
//     int bz = static_cast<int>(round(target.z));

//     if (bz >= 0 && bz < blockGrid.depth() &&
//         by >= 0 && by < blockGrid.rows() &&
//         bx >= 0 && bx < blockGrid.cols()) {
//         if (!blockGrid.at(bz, by, bx)) {
//             Vector3 blockPos(static_cast<float>(bx), static_cast<float>(by), static_cast<float>(bz));
//             BlockTextureSet btexture = texManager.getBlockTextures(blockType);
//             blockGrid.at(bz, by, bx) = std::make_shared<Block>(blockType, blockPos, blockSize, btexture);
//             std::cout << "Placed block at (" << bx << ", " << by << ", " << bz << ")\n";
//             glutPostRedisplay();
//         } else {
//             std::cout << "Cannot place block: position occupied.\n";
//         }
//     } else {
//         std::cout << "Cannot place block: out of bounds.\n";
//     }
// }

// Remove
void Player::remove(const Vector3& target) {
    int x = static_cast<int>(target.x);
    int y = static_cast<int>(target.y);
    int z = static_cast<int>(target.z);

    std::cout << "placing";
    if (x >= 0 && x < static_cast<int>(blockGrid.cols()) &&
        y >= 0 && y < static_cast<int>(blockGrid.rows()) &&
        z >= 0 && z < static_cast<int>(blockGrid.depth())) {

        blockGrid.at(z, y, x) = nullptr;  // urutan z, y, x sesuai Mat3d
    }
}

void Player::place(const Vector3& target) {
    auto& texManager = TextureManager::getInstance();

    // Block we are pointing at
    int hitX = static_cast<int>(std::floor(target.x));
    int hitY = static_cast<int>(std::floor(target.y));
    int hitZ = static_cast<int>(std::floor(target.z));

    // Face normal from look direction
    Vector3 normal(0, 0, 0);

    // Determine the most dominant axis in look direction
    float absX = std::abs(lookDirection.x);
    float absY = std::abs(lookDirection.y);
    float absZ = std::abs(lookDirection.z);

    if (absX >= absY && absX >= absZ)
        normal.x = (lookDirection.x > 0) ? -1 : 1;
    else if (absY >= absX && absY >= absZ)
        normal.y = (lookDirection.y > 0) ? -1 : 1;
    else
        normal.z = (lookDirection.z > 0) ? -1 : 1;

    // New block position: adjacent to the face we’re looking at
    int placeX = hitX + static_cast<int>(normal.x);
    int placeY = hitY + static_cast<int>(normal.y);
    int placeZ = hitZ + static_cast<int>(normal.z);

    // Debug output to confirm coordinates
    // std::cout << "Hit block: (" << hitX << ", " << hitY << ", " << hitZ << ")\n";
    // std::cout << "Look dir: (" << lookDirection.x << ", " << lookDirection.y << ", " << lookDirection.z << ")\n";
    // std::cout << "Place at: (" << placeX << ", " << placeY << ", " << placeZ << ")\n";

    // Bounds check
    if (placeX >= 0 && placeX < static_cast<int>(blockGrid.cols()) &&
        placeY >= 0 && placeY < static_cast<int>(blockGrid.rows()) &&
        placeZ >= 0 && placeZ < static_cast<int>(blockGrid.depth())) {

        if (!blockGrid.at(placeZ, placeY, placeX)) {
            BlockType type = hand[handindex];
            Vector3 pos(placeX, placeY, placeZ);
            Vector3 size(1.0f, 1.0f, 1.0f);
            BlockTextureSet textures = texManager.getBlockTextures(type);

            auto newBlock = std::make_shared<Block>(type, pos, size, textures);
            blockGrid.at(placeZ, placeY, placeX) = newBlock;

            std::cout << "Block placed.\n";
        } else {
            std::cout << "Block already exists at target location.\n";
        }
    } else {
        std::cout << "Target location out of bounds.\n";
    }
}

void Player::nextitem(){
    handindex += 1;
    handindex = handindex % 11;
    std::cout << "Current hand" << handindex << std::endl;
}

void Player::previtem(){
    handindex -= 1;
    if (handindex < 0) {
        handindex = 0;
    }
    std::cout << "Current hand" << handindex << std::endl;
}

Vector3 Player::getBlockInFront(float distance) const {
    Vector3 dir = lookDirection.normalized();
    Vector3 target = position + dir * distance;
    return Vector3(
        static_cast<int>(target.x),
        static_cast<int>(target.y),
        static_cast<int>(target.z)
    );
}


void Player::update(float deltaTime) {
    // float gravity = -9.8f;
    // velocity.y += gravity * deltaTime;
    position += velocity * deltaTime;

    // if (position.y < 0.0f) {
    //     position.y = 0.0f;
    //     velocity.y = 0.0f;
    // }
    glutPostRedisplay();
}

void Player::look(float deltaX, float deltaY) {
    float yawAngle = deltaX * mouseSensitivity;
    Vector3 up(0.0f, 1.0f, 0.0f);

    // Refactor: Use member function `rotate`
    lookDirection = lookDirection.rotate(yawAngle, up).normalized();

    float pitchAngle = deltaY * mouseSensitivity;
    // Refactor: Use member function `cross`
    // The cross product of lookDirection and up gives the right vector.
    // Ensure the order is correct for a right-handed coordinate system
    // (lookDirection x up) should give the right vector.
    Vector3 right = lookDirection.cross(up).normalized();

    // Refactor: Use member function `rotate`
    lookDirection = lookDirection.rotate(pitchAngle, right).normalized();

    // Refactor: Use member function `dot`
    float upDot = lookDirection.dot(Vector3(0.0f, 1.0f, 0.0f));
    if (std::abs(upDot) > 0.99f) {
        lookDirection.y = std::copysign(std::sqrt(1.0f - 0.99f * 0.99f), lookDirection.y);
        lookDirection = lookDirection.normalized();
    }
    glutPostRedisplay();
}

Vector3 Player::getClickedGroundCoordinate(int mouseX, int mouseY, int windowWidth, int windowHeight) {
    Vector3 origin = this->position;
    Vector3 dir = this->lookDirection.normalized();

    int bx = static_cast<int>(std::floor(origin.x));
    int by = static_cast<int>(std::floor(origin.y));
    int bz = static_cast<int>(std::floor(origin.z));

    int stepX = (dir.x > 0) ? 1 : -1;
    int stepY = (dir.y > 0) ? 1 : -1;
    int stepZ = (dir.z > 0) ? 1 : -1;

    // Distance along ray to next voxel boundary on each axis
    float nextVoxelBoundaryX = (stepX > 0) ? (bx + 1.0f) : bx * 1.0f;
    float nextVoxelBoundaryY = (stepY > 0) ? (by + 1.0f) : by * 1.0f;
    float nextVoxelBoundaryZ = (stepZ > 0) ? (bz + 1.0f) : bz * 1.0f;

    float tMaxX = (dir.x != 0) ? (nextVoxelBoundaryX - origin.x) / dir.x : std::numeric_limits<float>::infinity();
    float tMaxY = (dir.y != 0) ? (nextVoxelBoundaryY - origin.y) / dir.y : std::numeric_limits<float>::infinity();
    float tMaxZ = (dir.z != 0) ? (nextVoxelBoundaryZ - origin.z) / dir.z : std::numeric_limits<float>::infinity();

    float tDeltaX = (dir.x != 0) ? 1.0f / std::abs(dir.x) : std::numeric_limits<float>::infinity();
    float tDeltaY = (dir.y != 0) ? 1.0f / std::abs(dir.y) : std::numeric_limits<float>::infinity();
    float tDeltaZ = (dir.z != 0) ? 1.0f / std::abs(dir.z) : std::numeric_limits<float>::infinity();

    const float maxDist = 5.0f;
    float traveled = 0.0f;

    // Check if starting inside a block:
    if (bx >= 0 && bx < blockGrid.cols() &&
        by >= 0 && by < blockGrid.rows() &&
        bz >= 0 && bz < blockGrid.depth()) {
        if (blockGrid.at(bz, by, bx)) {
            return Vector3(bx, by, bz);
        }
    }

    while (traveled <= maxDist) {
        // Find axis with smallest tMax and step in that direction
        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) {
                bx += stepX;
                traveled = tMaxX;
                tMaxX += tDeltaX;
            } else {
                bz += stepZ;
                traveled = tMaxZ;
                tMaxZ += tDeltaZ;
            }
        } else {
            if (tMaxY < tMaxZ) {
                by += stepY;
                traveled = tMaxY;
                tMaxY += tDeltaY;
            } else {
                bz += stepZ;
                traveled = tMaxZ;
                tMaxZ += tDeltaZ;
            }
        }

        // Check bounds and block presence
        if (bx >= 0 && bx < blockGrid.cols() &&
            by >= 0 && by < blockGrid.rows() &&
            bz >= 0 && bz < blockGrid.depth()) {
            if (blockGrid.at(bz, by, bx)) {
                return Vector3(bx, by, bz);
            }
        } else {
            // Outside bounds - no block hit
            return Vector3(-1, -1, -1);
        }
    }

    // No block found within max distance
    return Vector3(-1, -1, -1);
}

