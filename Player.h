#pragma once
#include "vector3.h"
#include "block.h"
#include "textureManager.h" 
#include <array>
#include <memory>
#include <iostream>

static constexpr size_t INVENTORY_SLOTS = 36;

class Player {
public:
    Player()
        : position(0, 0, 0),
          inventoryIndex(0) {
        // initialize empty inventory
        inventory.fill(nullptr);
    }

    ~Player() {
        // free blocks in inventory if owned
        for (auto& slot : inventory) {
            if (slot) {
                delete slot;
                slot = nullptr;
            }
        }
    }

    // Move based on keyboard/mouse input vector
    void move(const Vector3& delta) {
        position += delta;
        std::cout << "Player moved to (" 
                  << position.x << ", "
                  << position.y << ", "
                  << position.z << ")\n";
    }

    // Place a block from inventory at target position
    bool place(const Vector3& target) {
        if (!inventory[inventoryIndex]) return false;

        auto blockType = inventory[inventoryIndex]->getType();
        auto blockSize = inventory[inventoryIndex]->getSize();

        auto newBlock = new Block(
            blockType,
            target,
            blockSize,
            TextureManager::getInstance().getBlockTextures(blockType)
        );

        std::cout << "Placed block of type " << static_cast<int>(newBlock->getType()) << " at ("
                  << target.x << ", " << target.y << ", " << target.z << ")\n";

        delete inventory[inventoryIndex];
        inventory[inventoryIndex] = nullptr;
        return true;
    }

    // Break block in the world and optionally add to inventory
    void breakBlock(Block* blockInWorld) {
        if (!blockInWorld) return;
        blockInWorld->onBreak();
        addToInventory(blockInWorld->getType(), blockInWorld->getSize());
        delete blockInWorld;
    }

    void selectNextSlot() {
        inventoryIndex = (inventoryIndex + 1) % INVENTORY_SLOTS;
    }

    void selectPreviousSlot() {
        inventoryIndex = (inventoryIndex + INVENTORY_SLOTS - 1) % INVENTORY_SLOTS;
    }

private:
    Vector3 position;
    std::array<Block*, INVENTORY_SLOTS> inventory;
    size_t inventoryIndex;

    void addToInventory(BlockType type, const Vector3& size) {
        for (auto& slot : inventory) {
            if (!slot) {
                slot = new Block(
                    type,
                    Vector3(),
                    size,
                    TextureManager::getInstance().getBlockTextures(type)
                );
                std::cout << "Added block type " << static_cast<int>(type) << " to inventory.\n";
                return;
            }
        }
        std::cout << "Inventory full! Cannot add block.\n";
    }
};
