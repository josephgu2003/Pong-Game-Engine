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
#include "Ability.hpp"
#include <vector>

//for abilities and fighting people, prob do some stuf with buffs, debuffs
// lvl blah blah

class Actor;

class CombatComponent : public Component {
private:
  //  std::weak_ptr<Actor> biggestTarget;
    std::vector<std::shared_ptr<Ability>> abilityQ;
    std::weak_ptr<Ability> affecting; 
public:
      
    template <typename AbilityType, typename... Args>
    void newAbility(Args&& ... arg) {
        abilityQ.emplace_back(std::make_shared<AbilityType>(arg...)); 
    }

    CombatComponent(Actor& actor);
    void tick() override;
    void newAbility(const std::shared_ptr<Ability>& ab);
    std::vector<std::shared_ptr<Ability>>& getAbilityQ();
    void affect(const std::shared_ptr<Ability>& ab);
    std::weak_ptr<Ability> getAffecting();
//    void setBigTarget(const std::shared_ptr<Actor>& ac);
 //   std::weak_ptr<Actor>& getBigTarget();
    void clearAffecting();
    bool hasTarget();
    bool QHasAbilities(); 
};
#endif /* CombatComponent_hpp */
