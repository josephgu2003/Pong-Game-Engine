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
#include <memory>
#include "Actor.hpp"
#include "Ability.hpp"

class CombatComponent : public Component {
private:
    std::shared_ptr<Ability> currentAbility;
    std::shared_ptr<Actor> biggestTarget;

    std::vector<std::shared_ptr<Ability>> abilityQ;
    std::shared_ptr<Ability> affecting;
public:
    CombatComponent();
    virtual void tick(Actor& actor, World& world) override;
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
