#pragma once
#include "Mat3.h"
#include "vector3.h"
#include <memory>
#include <vector>
#include <iostream>
#include "Block.h"

extern Mat3d<std::shared_ptr<Block>> blockGrid;

// Deklarasi fungsi yang sudah ada
void drawDoor(const Vector3& pos, const Vector3& size, const BlockTextureSet& textures, bool isBottomPart);
void drawCube(const Vector3& pos, const Vector3& size, GLuint textureID);
void initBlocks();

// TAMBAHKAN DEKLARASI INI:
void drawCubeWithTextures(const Vector3& pos, const Vector3& size, const BlockTextureSet& textures, const Block* block = nullptr);

// Jika ada fungsi lain yang diperlukan:
void placeDoor(const Vector3& bottomPos, BlockType doorType);