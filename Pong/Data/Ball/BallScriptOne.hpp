//
//  BallScriptOne.hpp
//  Pong
//
//  Created by Joseph Gu on 7/2/21.
//

#ifndef BallScriptOne_hpp
#define BallScriptOne_hpp

#include <stdio.h>
#include "ActorScript.hpp"
#include "Ability.hpp"

class BallScriptOne : public ActorScript {
    enum State {
        STATE_AGGRESSIVE,
        STATE_PASSIVE
    } state = STATE_PASSIVE;
    Ability* currentAbility;
public:
    virtual void tick() override;
    virtual void init() override;
};

#endif /* BallScriptOne_hpp */
