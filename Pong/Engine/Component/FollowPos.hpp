//
//  FollowPos.hpp
//  Pong
//
//  Created by Joseph Gu on 1/14/22.
//

#ifndef FollowPos_hpp
#define FollowPos_hpp

#include <stdio.h>
#include "Component.hpp"
#include "Positionable.hpp"


template <typename T>
class FollowPos : public Component {
    glm::vec3 offset;
    Positionable& target;
public:
    FollowPos(T& t, Positionable& following, glm::vec3 offset_) : Component(t), target(following) {
        offset = offset_;
        updatePriority = 4;
    }
    void tick() override {
        static_cast<T*>(actor)->setPos(target.getPos()+offset);
    } 
};
#endif /* FollowPos_hpp */
