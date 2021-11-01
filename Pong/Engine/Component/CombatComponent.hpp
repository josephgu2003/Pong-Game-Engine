//
//  CombatComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 6/17/21.
//

#ifndef CombatComponent_hpp
#define CombatComponent_hpp

#include <stdio.h>
#include "ActorComp.hpp"
#include <memory>
#include "Actor.hpp"
#include "Ability.hpp"

//for abilities and fighting people, prob do some stuf with buffs, debuffs
// lvl blah blah

class CombatComponent : public ActorComp {
private:
    std::shared_ptr<Ability> currentAbility;
    std::shared_ptr<Actor> biggestTarget;
    std::vector<std::shared_ptr<Ability>> abilityQ;
    std::shared_ptr<Ability> affecting;
public:
    CombatComponent(Actor& actor);
    virtual void tick() override; 
    void newAbility(const std::shared_ptr<Ability>& ab);
    std::vector<std::shared_ptr<Ability>>& getAbilityQ();
    void affect(std::shared_ptr<Ability> ab);
    std::shared_ptr<Ability> getAffecting();
    void setBigTarget(const std::shared_ptr<Actor>& ac);
    std::shared_ptr<Actor>& getBigTarget();
    void clearAffecting();
    bool hasTarget();
    bool QHasAbilities();
};
#endif /* CombatComponent_hpp */
