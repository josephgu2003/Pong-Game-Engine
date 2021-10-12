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

// an action, encapsulate a function as a class

class aiAction {
protected:
    bool complete; // may be switched on when 
public:
    aiAction();
    virtual void tick(Actor* actor) = 0;
};

#endif /* AIAction_hpp */
