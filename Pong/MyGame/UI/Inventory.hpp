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

class Inventory : public uiPiece, public Observer {
private:
    bool visible;
    Watch watch;
public:
    Inventory(glm::vec2 pos, glm::vec2 scaling);
    virtual void notify(const Subject& s, GameEvent ge) override;
    void draw(Renderer* r) override;
}; 

#endif /* Inventory_hpp */
