#pragma once
#include "Mat3.h"
#include "vector3.h"
#include <memory>
#include <vector>
#include <iostream>
#include "Block.h"

extern Mat3d<std::shared_ptr<Block>> blockGrid;

// Deklarasi fungsi yang sudah ada
void setBrightness(float brightness);
void initBlocks();
void placeDoor(const Vector3& bottomPos, BlockType doorType);
