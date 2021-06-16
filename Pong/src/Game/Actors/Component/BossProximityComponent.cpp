//
//  BossProximityComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 6/15/21.
//

#include "BossProximityComponent.hpp"
#include "Actor.hpp"
#include "Speech.hpp"
#include "World.hpp"
#include <vector>
#include <string>

void BossProximityComponent::trigger(Actor& actor, World& world) {
    if (cooldown <= 0) {
        transgressions++;
    std::vector<std::string> newLines = {"You shouldn't have come here...", "I will attack"};
    Speech* speech = new Speech(&world, &actor, 6, newLines);
    actor.abilityQ.push_back(speech);
        cooldown = 8;
        //glm::vec3 newDir = biggestTarget->getPos() - actor.getPos();
    } else {
        cooldown -= (float)glfwGetTime();
    }
}


