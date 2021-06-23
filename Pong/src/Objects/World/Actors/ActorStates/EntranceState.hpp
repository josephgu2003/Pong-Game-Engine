//
//  EntranceState.hpp
//  Pong
//
//  Created by Joseph Gu on 6/16/21.
//

#ifndef EntranceState_hpp
#define EntranceState_hpp

#include <stdio.h>
#include "ActorState.hpp"

class EntranceState : public ActorState {
public:
    virtual void tick() override;
};

#endif /* EntranceState_hpp */
