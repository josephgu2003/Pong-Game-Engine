//
//  Component.hpp
//  Pong
//
//  Created by Joseph Gu on 5/29/21.
//

#ifndef Component_hpp
#define Component_hpp

#include <stdio.h>
#include <memory>

class Componentable; 

class Component {
protected:
    Componentable* actor = nullptr;
    int updatePriority;
public:
    Component(Componentable& compUser_) {
        actor = &compUser_;
        updatePriority = 0;
    }
    
    virtual ~Component() {
        
    }
    
    int getUpdatePriority() {
        return updatePriority;
    }

    virtual void tick() = 0;
    
    Componentable* getParentObject();
};

#endif /* Component_hpp */
