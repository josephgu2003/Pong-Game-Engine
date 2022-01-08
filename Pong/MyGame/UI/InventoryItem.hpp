//
//  InventoryItem.hpp
//  Pong
//
//  Created by Joseph Gu on 1/2/22.
//

#ifndef InventoryItem_hpp
#define InventoryItem_hpp

#include <stdio.h>
#include "uiPiece.hpp"

class InventoryItem : public uiPiece {
public:
    InventoryItem(const std::string& icon, glm::vec2 pos, glm::vec2 scaling);
};
#endif /* InventoryItem_hpp */
