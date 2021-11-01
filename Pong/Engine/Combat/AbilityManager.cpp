//
//  AbilityManager.cpp
//  Pong
//
//  Created by Joseph Gu on 9/11/21.
//

#include "AbilityManager.hpp"
#include "CombatComponent.hpp"

AbilityManager::AbilityManager() {
    
}

void AbilityManager::tick() {

    if (abilities.size() > 0) {
        for (int i = 0; i < abilities.size(); i++) {
            if(abilities.at(i)->on == true) {
                abilities.at(i)->tick();
            }
            if(abilities.at(i)->on == false) {
                abilities.at(i).reset();
                abilities.erase(abilities.begin()+i);
            }
        }
    }
}


void AbilityManager::handleCombatComp(CombatComponent* cc) {
    std::vector<std::shared_ptr<Ability>>& q = cc->getAbilityQ();
    if (q.size() > 0) {
        for (int j = 0; j < q.size(); j ++) {
            std::shared_ptr<Ability> a = q.at(j);
            abilities.push_back(std::move(a));
            q.at(j)->call();
        } 
        q.clear();
    }
}
