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

// Place

void Player::place(const Vector3& target) {
    auto& texManager = TextureManager::getInstance();
    
    int x = static_cast<int>(target.x);
    int y = static_cast<int>(target.y);
    int z = static_cast<int>(target.z);

    if (x >= 0 && x < static_cast<int>(blockGrid.cols()) &&
        y >= 0 && y < static_cast<int>(blockGrid.rows()) &&
        z >= 0 && z < static_cast<int>(blockGrid.depth())) {

        if (!blockGrid.at(z, y, x)) { // hanya jika kosong
            BlockType type = BlockType::Grass;  // atau yang kamu mau
            Vector3 pos(x, y, z);
            Vector3 size(1.0f, 1.0f, 1.0f);

            BlockTextureSet textures = texManager.getBlockTextures(type); // fungsi yang sudah ada

            auto newBlock = std::make_shared<Block>(type, pos, size, textures);
            blockGrid.at(z, y, x) = newBlock;
        }
    }
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
    Vector3 dir = this->lookDirection;
    const float stepSize = 0.05f;
    const float maxDist = 5.0f;

    for (float t = 0; t < maxDist; t += stepSize) {
        Vector3 pos = origin + dir * t;

        int bx = static_cast<int>(floor(pos.x));
        int by = static_cast<int>(floor(pos.y));
        int bz = static_cast<int>(floor(pos.z));

        // std::cout << "Checking block at: (" << bx << ", " << by << ", " << bz << ")\n";

        if (bx < 0 || by < 0 || bz < 0 || bx >= blockGrid.cols() || by >= blockGrid.rows() || bz >= blockGrid.depth())
            continue;

        if (blockGrid.at(bz, by, bx)) {
            std::cout << "✅ Hit block at: (" << bx << ", " << by << ", " << bz << ")\n";
            return Vector3(bx, by, bz);
        }
    }
    // std::cout << "❌ No block hit.\n";
    return Vector3(-1, -1, -1);

}
