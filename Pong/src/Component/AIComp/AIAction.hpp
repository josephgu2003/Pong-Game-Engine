//
//  AIAction.hpp
//  Pong
//
//  Created by Joseph Gu on 10/2/21.
//

#ifndef AIAction_hpp
#define AIAction_hpp

#include <stdio.h>
#include "Actor.hpp"

class aiAction {
protected:
    float interest;
public:
    aiAction(float interest_);
    virtual void tick(Actor* actor) = 0;
    bool isDone();
};


#endif /* AIAction_hpp */
