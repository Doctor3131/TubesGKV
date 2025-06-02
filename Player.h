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

    // 
    void remove(const Vector3& target);
    Vector3 getBlockInFront(float distance = 5.0f) const;

    void nextitem();
    void previtem();
    void place(const Vector3& target);
    void update(float deltaTime); // Apply gravity
    void look(float deltaX, float deltaY); // Handle mouse look
    void setPosition(const Vector3& newPos);
    Vector3 getClickedGroundCoordinate(int mouseX, int mouseY, int windowWidth, int windowHeight);

private:
    Vector3 position;
    Vector3 velocity;
    BlockType hand[11] = {
        BlockType::Grass,
        BlockType::Dirt,
        BlockType::Wood,
        BlockType::Bookshelf,
        BlockType::Stone_bricks,
        BlockType::Crafting_table,
        BlockType::Ice,
        BlockType::Sun,
        BlockType::Sand,
        BlockType::Stone,
        BlockType::Quartz,
        // BlockType::Torch
    };
    int handindex = 0;
    float mouseSensitivity = 0.005f;
};
