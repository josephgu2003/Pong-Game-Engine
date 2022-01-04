//
//  Inventory.hpp
//  Pong
//
//  Created by Joseph Gu on 12/29/21.
//

#ifndef Inventory_hpp
#define Inventory_hpp

#include <stdio.h>
#include "uiPiece.hpp"
#include "Observer.hpp"
#include "Watch.hpp"
#include <set>
#include "InventoryComponent.hpp"
#include "InventoryItem.hpp"

constexpr unsigned int MAX_ITEMS = 1;

const static std::string ItemPaths[MAX_ITEMS] = {TEX_FISH};

class Inventory : public uiPiece, public Observer {
private:
    bool visible;
    Watch watch;
    glm::vec2 nextPos;
    glm::vec2 originalPos;
    float maxRowWidth;
    glm::vec2 pieceSize;
    std::set<int> loadedItems;
    inline void loadItem (int code) {
        std::string filepath = ItemPaths[code];
        std::shared_ptr<uiPiece> iconpiece = std::make_shared<InventoryItem>(filepath, nextPos, pieceSize);
        insertChild(iconpiece);
        float newNextX = nextPos.x + pieceSize.x;
        if (newNextX > maxRowWidth) nextPos = glm::vec2(originalPos.x,nextPos.y+pieceSize.y);
        else nextPos.x += pieceSize.x;
        loadedItems.insert(code);
    }
public:
    Inventory(std::weak_ptr<InventoryComponent>& invref, glm::vec2 pos, glm::vec2 scaling);
    virtual void notify(const Subject& s, GameEvent ge) override;
    void draw(Renderer* r) override;
}; 

#endif /* Inventory_hpp */
