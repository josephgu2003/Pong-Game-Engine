//
//  BossProximityComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 6/15/21.
//

#ifndef BossProximityComponent_hpp
#define BossProximityComponent_hpp

#include <stdio.h>
#include "BehaviorComponent.hpp"
#include "Ability.hpp"
#include <memory>

enum BehaviorState {
    STATE_PASSIVE,
    STATE_AGGRESSIVE
};

class BossBehaviorComponent : public BehaviorComponent {
    float timeTransgressed = 0;
    int step = 0;
    BehaviorState state = STATE_PASSIVE;
    Ability* currentAbility;
    bool stepBegun = false;
public:
    virtual void tick(Actor& actor, World& world) override;
    virtual void trigger(Actor& actor, World& world) override;
};

#endif /* BossProximityComponent_hpp */
