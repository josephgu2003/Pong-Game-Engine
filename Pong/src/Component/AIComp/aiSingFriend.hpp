//
//  aiSingFriend.hpp
//  Pong
//
//  Created by Joseph Gu on 10/17/21.
//

#ifndef aiSingFriend_hpp
#define aiSingFriend_hpp

#include <stdio.h>
#include "AIAction.hpp"
#include "Watch.hpp"

class aiSingFriend: public aiAction {
private:
    Watch myWatch;
    Actor* target = NULL;
public:
    aiSingFriend(float i, Actor* actor);
    virtual void tick(Actor* actor) override;
};

#endif /* aiSingFriend_hpp */
