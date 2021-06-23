//
//  CombatComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 6/17/21.
//

#ifndef CombatComponent_hpp
#define CombatComponent_hpp

#include <stdio.h>
#include "Component.hpp"

class CombatComponent : public Component {
public:
    CombatComponent();
    virtual void tick(Actor& actor, World& world) override;
};
#endif /* CombatComponent_hpp */
