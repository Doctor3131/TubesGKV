#pragma once
#include "vector3.h"
#include "block.h"
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
        // In real game: add block to world
        auto newBlock = new Block(inventory[inventoryIndex]->getType(), target, inventory[inventoryIndex]->getSize());
        std::cout << "Placed block of type " << static_cast<int>(newBlock->getType()) << " at ("
                  << target.x << ", " << target.y << ", " << target.z << ")\n";
        // decrease count or remove from inventory
        delete inventory[inventoryIndex];
        inventory[inventoryIndex] = nullptr;
        return true;
    }

    // Break block in the world and optionally add to inventory
    void breakBlock(Block* blockInWorld) {
        if (!blockInWorld) return;
        blockInWorld->onBreak();
        // pick up block
        addToInventory(blockInWorld->getType(), blockInWorld->getSize());
        // remove from world
        delete blockInWorld;
    }

    // Revisi nanti Unggul akan revisi
    // Cycle inventory index
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

    // Helper to add a block item to inventory
    void addToInventory(BlockType type, const Vector3& size) {
        for (auto& slot : inventory) {
            if (!slot) {
                slot = new Block(type, Vector3(), size);
                std::cout << "Added block type " << static_cast<int>(type) << " to inventory.\n";
                return;
            }
        }
        std::cout << "Inventory full! Cannot add block.\n";
    }
};