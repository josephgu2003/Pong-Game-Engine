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

    
    template <typename T>
    T* getComponent()
    {
        for (int i = 0 ; i < components.size(); i++) {
            if (dynamic_cast<T*>(components.at(i).get()) != NULL) {
                return static_cast<T*>(components.at(i).get());
            }
        }

            return nullptr; // make a null component or something

    }
};

#endif /* Componentable_hpp */
