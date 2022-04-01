//
//  InventoryComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 1/2/22.
//
 
#include "InventoryComponent.hpp"
#include "Actor.hpp"

InventoryComponent::InventoryComponent(Actor& actor, const std::string* gameitemtable) : Component(actor) {
    gameItemTable = gameitemtable;
}
    
void InventoryComponent::tick() {
    
}  

const std::vector<GameItem>& InventoryComponent::getItems() {
    return gameItems;
}

void InventoryComponent::insertItem(unsigned int code) {
    gameItems.emplace_back(gameItemTable[code], code);
    notifyAll(ACQUIRE_NEW_ITEM); 
    recentItem = code;
}

unsigned int InventoryComponent::getLastInsertedItem() const {
    return recentItem; 
}
