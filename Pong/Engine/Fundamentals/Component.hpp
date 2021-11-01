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

enum CompType {
    AI,
    CHAR,
    LIFE,
    NAME,
    COMBAT,
    ANIM,
    PHYSICS,
    GRAPHICS
};

template <typename T>  // ???
class Component {
protected:
    T* actor = nullptr;
    CompType type;
public:
    Component(T& compUser_) {
        actor = &compUser_;
    }

    CompType getType() {
        return type;
    }

    virtual void tick() = 0;
};

#endif /* Component_hpp */
