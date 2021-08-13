//
//  Componentable.hpp
//  Pong
//
//  Created by Joseph Gu on 7/31/21.
//

#ifndef Componentable_hpp
#define Componentable_hpp

#include <stdio.h>
#include <vector>
#include "Component.hpp"
#include <memory>

class Componentable {
private:
    Actor* actor= NULL;
    World* world = NULL;
    std::vector<std::shared_ptr<Component>> components;
public:
    Componentable();
    void init(Actor* actor, World* world);
    void tick();
    virtual void addComp(const std::shared_ptr<Component>& comp);
    virtual void deleteComp(CompType type);
    std::shared_ptr<Component> getComp(CompType type);
};

#endif /* Componentable_hpp */
