#include "vector3.h"
#include "block.h"
#include "player.h"
#include <iostream>

int main() {
    Player player;
    std::cout << "-- Testing Player move --\n";
    player.move(Vector3(1.0f, 0.0f, 0.0f));
    player.move(Vector3(0.0f, 1.0f, 0.0f));

    std::cout << "\n-- Testing Inventory & Place/Break --\n";
    // Simulate picking up a block by directly adding to inventory
    // We use breakBlock on a temporary block to add it
    Block* worldBlock = new Block(BlockType::Grass, Vector3(2, 2, 2), Vector3(1,1,1));
    player.breakBlock(worldBlock);

    // Try placing the block back at a new location
    bool placed = player.place(Vector3(5, 5, 5));
    std::cout << "Place returned: " << placed << "\n";

    std::cout << "\n-- Testing Inventory Slot Cycling --\n";
    player.selectNextSlot();
    player.selectNextSlot();
    player.selectPreviousSlot();

    std::cout << "\n-- All tests completed --\n";
    return 0;
}
