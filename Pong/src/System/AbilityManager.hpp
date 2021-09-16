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

class AbilityManager {
private:
    std::vector<std::shared_ptr<Ability>> abilities;
    std::vector<std::shared_ptr<Actor>> actors;
public:
    AbilityManager();
    void tick();
    void insertActor(const std::shared_ptr<Actor>& actor);
};
#endif /* AbilityManager_hpp */
