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
 
class InventoryComponent : public Component, public Subject {
private:
    std::vector<GameItem> gameItems;
public:
    using Component::Component;
    const std::vector<GameItem>& getItems();
    void tick() override;
    void insertItem(unsigned int code);
};

#endif /* InventoryComponent_hpp */
