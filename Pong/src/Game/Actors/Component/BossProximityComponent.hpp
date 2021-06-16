//
//  BossProximityComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 6/15/21.
//

#ifndef BossProximityComponent_hpp
#define BossProximityComponent_hpp

#include <stdio.h>
#include "ProximityComponent.hpp"

class BossProximityComponent : public ProximityComponent {
    float cooldown = 0;
    int transgressions = 0;
public:
    virtual void trigger(Actor& actor, World& world) override;
};

#endif /* BossProximityComponent_hpp */
