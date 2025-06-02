// Player.h
#pragma once
#include "vector3.h"
#include "block.h"
#include "textureManager.h" 
#include "renderChunks.h"
#include <array>
#include <memory>
#include <iostream>
#include <GL/glut.h>

class Player {
public:
    Vector3 lookDirection;

    Player();

    const Vector3& getPosition() const { return position; }

    void moveForward(float speed);
    void moveBackward(float speed);
    void moveLeft(float speed);
    void moveRight(float speed);
    void moveUp(float speed); // For jumping
    void moveDown(float speed);

    void place(const Vector3& target);
    void update(float deltaTime); // Apply gravity
    void look(float deltaX, float deltaY); // Handle mouse look
    void setPosition(const Vector3& newPos);
    Vector3 getClickedGroundCoordinate(int mouseX, int mouseY, int windowWidth, int windowHeight);

private:
    Vector3 position;
    Vector3 velocity;
    float mouseSensitivity = 0.005f;
};
