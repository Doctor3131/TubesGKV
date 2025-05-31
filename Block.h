#pragma once
#include <glad/glad.h>
#include "Vector3.h"
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
    // Constructor dengan nilai default untuk textureID dan Desktructor
    Block(BlockType type, const Vector3& pos, const Vector3& size, GLuint textureID = 0)
        : type(type), position(pos), size(size), textureID(textureID) {
    }

    virtual ~Block() {
        // Cleanup if needed
    }

    // Called when the block is destroyed
    // On break harus komunikasi ke world state 
    //      -> antara player yang ngirim signal ke world atau block ngirim signal ke world
    virtual void onBreak() {
        std::cout << "Block broken at (" 
                  << position.x << ", "
                  << position.y << ", "
                  << position.z << ")\n";
        // Drop item or update world state
    }

    // Accessors
    BlockType getType() const { return type; }
    const Vector3& getPosition() const { return position; }
    const Vector3& getSize() const { return size; }

    // Getter and Setter untuk textureID
    GLuint getTextureID() const { return textureID; }
    void setTextureID(GLuint id) { textureID = id; }

protected:
    BlockType type;
    Vector3 position;
    Vector3 size;
    GLuint textureID;  // ID untuk tekstur
};