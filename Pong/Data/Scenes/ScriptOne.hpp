//
//  ScriptOne.hpp
//  Pong
//
//  Created by Joseph Gu on 7/9/21.
//

#ifndef ScriptOne_hpp
#define ScriptOne_hpp

#include <stdio.h>
#include "Script.hpp"
#include "PlayerHero.hpp"
#include "World.hpp"
#include "Ball.hpp"

class ScriptOne : public Script {
    PlayerHero* pHero;
    World* world;
    Ball* ball;
    
    enum State {
        STATE_AGGRESSIVE,
        STATE_PASSIVE
    } state = STATE_PASSIVE;
    Ability* currentAbility;
public:
    virtual void tick() override;
    virtual void init(Game* game) override;
};
#endif /* ScriptOne_hpp */
