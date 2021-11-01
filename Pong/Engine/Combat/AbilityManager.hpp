//
//  AbilityManager.hpp
//  Pong
//
//  Created by Joseph Gu on 9/11/21.
//

#ifndef AbilityManager_hpp
#define AbilityManager_hpp

#include <stdio.h>
#include "Ability.hpp"
#include <memory> 
#include "Actor.hpp"

class CombatComponent;
 
class AbilityManager {
private:
    std::vector<std::shared_ptr<Ability>> abilities;
public:
    AbilityManager();
    void tick();
    void handleCombatComp(CombatComponent* cc);
};
#endif /* AbilityManager_hpp */
