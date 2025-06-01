#pragma once
#include <glad/glad.h>
#include "Vector3.h"
#include <iostream>
#include <unordered_map>
#include <string>

// Define block types with textures
enum class BlockType {
    Grass,
    Dirt,
    Stone,
    Wood,
    Sand,
    Berry,
    Bookshelf,
    Flower_leaves,
    Flower,
    Leaves,
    Quartz,
    Stone_bricks,
    tumbuhan,
    Crafting_table,
    Ice,
    Door,
    Sun
    // add more types as needed
};

// Define block face
enum class BlockFace{
    FRONT = 0,
    BACK = 1,
    LEFT = 2,
    RIGHT = 3,
    TOP = 4,
    BOTTOM = 5
};


// Set Block Texture
struct BlockTextureSet {
    GLuint front = 0;
    GLuint back = 0;
    GLuint left = 0;
    GLuint right = 0;
    GLuint top = 0;
    GLuint bottom = 0;

    // Constructor untuk block dengan texture sama semua sisi
    BlockTextureSet(GLuint allSides) : 
        front(allSides), back(allSides), left(allSides), 
        right(allSides), top(allSides), bottom(allSides) {}

    // Constructor untuk block dengan texture berbeda
    BlockTextureSet(GLuint side, GLuint topTex, GLuint bottomTex) :
        front(side), back(side), left(side), right(side), 
        top(topTex), bottom(bottomTex) {}

    // Constructor lengkap untuk kontrol penuh
    BlockTextureSet(GLuint f, GLuint b, GLuint l, GLuint r, GLuint t, GLuint bot) :
        front(f), back(b), left(l), right(r), top(t), bottom(bot) {}

    GLuint getTexture(BlockFace face) const {
        switch(face) {
            case BlockFace::FRONT: return front;
            case BlockFace::BACK: return back;
            case BlockFace::LEFT: return left;
            case BlockFace::RIGHT: return right;
            case BlockFace::TOP: return top;
            case BlockFace::BOTTOM: return bottom;
            default: return front;
        }
    }
};





// Class Block
class Block {
public:
    // Constructor dengan nilai default untuk textureID dan Desktructor
    Block(BlockType type, const Vector3& pos, const Vector3& size, const BlockTextureSet& textures)
        : type(type), position(pos), size(size), textures(textures), isMultiBlock(false) {}

    // Constructor untuk door (multi-block)
    Block(BlockType type, const Vector3& pos, const Vector3& size, const BlockTextureSet& textures, bool isBottom)
        : type(type), position(pos), size(size), textures(textures), 
          isMultiBlock(true), isBottomPart(isBottom) {}


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
    const BlockTextureSet& getTextures() const { return textures; }

    bool isMultiBlockType() const { return isMultiBlock; }
    bool isBottomBlock() const { return isBottomPart; }

    // Getter and Setter untuk textureID
    GLuint getTextureID() const { return textureID; }
    void setTextureID(GLuint id) { textureID = id; }



    // Update shouldRenderFace di Block.h
    bool shouldRenderFace(BlockFace face) const {
        return true; // Normal block render semua face
    }

protected:
    BlockType type;
    Vector3 position;
    Vector3 size;
    GLuint textureID;
    BlockTextureSet textures;
    bool isMultiBlock = false;
    bool isBottomPart = true;
};