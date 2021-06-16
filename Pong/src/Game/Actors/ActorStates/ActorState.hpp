//
//  ActorState.hpp
//  Pong
//
//  Created by Joseph Gu on 6/16/21.
//

#ifndef ActorState_hpp
#define ActorState_hpp

#include <stdio.h>

class ActorState {
public:
    virtual void tick() = 0;
};


#endif /* ActorState_hpp */
