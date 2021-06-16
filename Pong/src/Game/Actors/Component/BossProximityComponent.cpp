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
    std::vector<std::string> newLines = {"Oh? So you are approaching me?", "This is transgression " + std::to_string(transgressions)};
    Speech* speech = new Speech(&world, &actor, 6, newLines);
    actor.abilityQ.push_back(speech);
        cooldown = 13;
        glm::vec2 xz = 
        actor.orient();
    } else {
        cooldown -= (float)glfwGetTime();
    }
}


