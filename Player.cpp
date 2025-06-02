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
    auto& texManager = TextureManager::getInstance();

    int bx = static_cast<int>(round(target.x));
    int by = static_cast<int>(round(target.y));
    int bz = static_cast<int>(round(target.z));

    if (bz >= 0 && bz < blockGrid.depth() &&
        by >= 0 && by < blockGrid.rows() &&
        bx >= 0 && bx < blockGrid.cols()) {
        if (!blockGrid.at(bz, by, bx)) {
            Vector3 blockPos(static_cast<float>(bx), static_cast<float>(by), static_cast<float>(bz));
            BlockTextureSet btexture = texManager.getBlockTextures(blockType);
            blockGrid.at(bz, by, bx) = std::make_shared<Block>(blockType, blockPos, blockSize, btexture);
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

Vector3 Player::getClickedGroundCoordinate(int mouseX, int mouseY, int windowWidth, int windowHeight) {
    // Get matrices and viewport
    GLdouble modelview[16], projection[16];
    GLint viewport[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    // Convert mouse to OpenGL viewport coordinates (origin bottom-left)
    GLfloat winX = static_cast<GLfloat>(mouseX);
    GLfloat winY = static_cast<GLfloat>(viewport[3] - mouseY);

    GLdouble nearX, nearY, nearZ;
    GLdouble farX, farY, farZ;

    gluUnProject(winX, winY, 0.0, modelview, projection, viewport, &nearX, &nearY, &nearZ);
    gluUnProject(winX, winY, 1.0, modelview, projection, viewport, &farX, &farY, &farZ);

    Vector3 rayOrigin(nearX, nearY, nearZ);
    Vector3 rayDir = Vector3(farX - nearX, farY - nearY, farZ - nearZ).normalized();

    float closestT = std::numeric_limits<float>::max();
    Vector3 closestBlockPos(-1, -1, -1);

    auto RayIntersectsBox = [](const Vector3& rayOrig, const Vector3& rayDir,
                               const Vector3& boxMin, const Vector3& boxMax,
                               float& tNear) -> bool {
        float tmin = (boxMin.x - rayOrig.x) / rayDir.x;
        float tmax = (boxMax.x - rayOrig.x) / rayDir.x;
        if (tmin > tmax) std::swap(tmin, tmax);

        float tymin = (boxMin.y - rayOrig.y) / rayDir.y;
        float tymax = (boxMax.y - rayOrig.y) / rayDir.y;
        if (tymin > tymax) std::swap(tymin, tymax);

        if ((tmin > tymax) || (tymin > tmax))
            return false;

        if (tymin > tmin)
            tmin = tymin;
        if (tymax < tmax)
            tmax = tymax;

        float tzmin = (boxMin.z - rayOrig.z) / rayDir.z;
        float tzmax = (boxMax.z - rayOrig.z) / rayDir.z;
        if (tzmin > tzmax) std::swap(tzmin, tzmax);

        if ((tmin > tzmax) || (tzmin > tmax))
            return false;

        if (tzmin > tmin)
            tmin = tzmin;
        if (tzmax < tmax)
            tmax = tzmax;

        if (tmax < 0)
            return false;

        tNear = tmin >= 0 ? tmin : tmax;
        return true;
    };

    for (int z = 0; z < blockGrid.depth(); ++z) {
        for (int y = 0; y < blockGrid.rows(); ++y) {
            for (int x = 0; x < blockGrid.cols(); ++x) {
                if (!blockGrid.at(z, y, x)) continue;  // no block here

                Vector3 boxMin(x, y, z);
                Vector3 boxMax(x + 1.0f, y + 1.0f, z + 1.0f);

                float tNear;
                if (RayIntersectsBox(rayOrigin, rayDir, boxMin, boxMax, tNear)) {
                    if (tNear < closestT) {
                        closestT = tNear;
                        closestBlockPos = Vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
                    }
                }
            }
        }
    }

    if (closestBlockPos.x == -1 && closestBlockPos.y == -1 && closestBlockPos.z == -1) {
        return Vector3(-1, -1, -1);
    }

    std::cout << "Player clicked block at: (" << closestBlockPos.x << ", " << closestBlockPos.y << ", " << closestBlockPos.z << ")\n";
    return closestBlockPos;
}
