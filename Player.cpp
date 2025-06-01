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

void Player::place(const Vector3& target) {
    Vector3 blockSize(1.0f, 1.0f, 1.0f);
    BlockType blockType = BlockType::Wood;

    int bx = static_cast<int>(round(target.x));
    int by = static_cast<int>(round(target.y));
    int bz = static_cast<int>(round(target.z));

    if (bz >= 0 && bz < blockGrid.depth() &&
        by >= 0 && by < blockGrid.rows() &&
        bx >= 0 && bx < blockGrid.cols()) {
        if (!blockGrid.at(bz, by, bx)) {
            Vector3 blockPos(static_cast<float>(bx), static_cast<float>(by), static_cast<float>(bz));
            blockGrid.at(bz, by, bx) = std::make_shared<Block>(blockType, blockPos, blockSize);
            std::cout << "Placed block at (" << bx << ", " << by << ", " << bz << ")\n";
            glutPostRedisplay();
        } else {
            std::cout << "Cannot place block: position occupied.\n";
        }
    } else {
        std::cout << "Cannot place block: out of bounds.\n";
    }
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