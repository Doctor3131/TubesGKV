#pragma once
#include "vector3.h"
#include <iostream>

// Define block types with textures
enum class BlockType {
    Grass,
    Dirt,
    Stone,
    Wood,
    Sand,
    // add more types as needed
};

class Block {
public:
    // Constructors & Destructor
    Block(BlockType type, const Vector3& pos, const Vector3& size)
        : type(type), position(pos), size(size) {
        // load texture based on type
    }

    virtual ~Block() {
        // cleanup if needed
    }

    // Called when the block is destroyed
    virtual void onBreak() {
        std::cout << "Block broken at (" 
                  << position.x << ", "
                  << position.y << ", "
                  << position.z << ")\n";
        // drop item or update world state
    }

    // Accessors
    BlockType getType() const { return type; }
    const Vector3& getPosition() const { return position; }
    const Vector3& getSize() const { return size; }

protected:
    BlockType type;
    Vector3 position;
    Vector3 size;
};