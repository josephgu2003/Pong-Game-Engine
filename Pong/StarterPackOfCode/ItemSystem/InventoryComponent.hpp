//
//  InventoryComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 1/2/22.
//

#ifndef InventoryComponent_hpp
#define InventoryComponent_hpp

#include <stdio.h>
#include "Component.hpp"
#include "GameItem.hpp"
#include "Subject.hpp"

class Actor;
 
class InventoryComponent : public Component, public Subject {
private:
    std::vector<GameItem> gameItems;
    unsigned int recentItem = -1;
    const std::string* gameItemTable;
public:
    InventoryComponent(Actor& actor, const std::string* gameitemtable);
    const std::vector<GameItem>& getItems();

    void tick() override;
    void insertItem(unsigned int code);
    unsigned int getLastInsertedItem() const;
};

#endif /* InventoryComponent_hpp */
