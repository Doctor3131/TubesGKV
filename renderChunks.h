#pragma once

#include "Mat3.h"
#include "vector3.h"
#include <memory>
#include <vector>
#include <iostream>

#include "Block.h"

// extern Mat3d<std::shared_ptr<Block>> blockGrid(3, 3, 3);
extern Mat3d<std::shared_ptr<Block>> blockGrid;

void drawCube(const Vector3& pos, const Vector3& size, GLuint textureID);

void initBlocks();
