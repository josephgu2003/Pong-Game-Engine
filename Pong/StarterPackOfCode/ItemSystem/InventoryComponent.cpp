//
//  InventoryComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 1/2/22.
//

#include "InventoryComponent.hpp" 
 
void InventoryComponent::tick() {
    
} 

const std::vector<GameItem>& InventoryComponent::getItems() {
    return gameItems;
}

void InventoryComponent::insertItem(unsigned int code) {
    gameItems.emplace_back("need to DI a game item table or something", code);
}
