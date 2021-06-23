//
//  BossProximityComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 6/15/21.
//

#include "BossBehaviorComponent.hpp"
#include "Actor.hpp"
#include "Speech.hpp"
#include "World.hpp"
#include <vector>
#include <string>
#include "FallingLetters.hpp"
#include "Dialogue.hpp"

void BossBehaviorComponent::tick(Actor& actor, World& world) {
    world.informProximityComponent(actor, *this);
    if (!somethingNear) {
        state = STATE_PASSIVE;
        if (currentAbility != NULL)
        currentAbility->on = false;
        stepBegun = false;
        timeTransgressed = 0;
    }
    if (somethingNear) {
        trigger(actor,world);
        somethingNear= false;
    }
}

void BossBehaviorComponent::trigger(Actor& actor, World& world) {
    switch (state) {
        case STATE_PASSIVE: {
        
        actor.posVec += 0.01f*glm::vec3(0,sin(timeTransgressed),0);
    timeTransgressed += (float)glfwGetTime();
    
    if (timeTransgressed > 3) state = STATE_AGGRESSIVE;
            break;
        };
        
case STATE_AGGRESSIVE:
        actor.turnTowards(biggestTarget->getPos()-actor.getPos());
        actor.posVec += 0.01f*glm::vec3(0,sin(timeTransgressed),0);
        timeTransgressed += (float)glfwGetTime();
            
        if (step == 0 && !stepBegun) {
  //  std::vector<std::string> newLines = {"From the emptiness of white, black ink dictates what is", "And what should not be", "Our battle is already settled - ", "I win, you lose, black, white"};
  //  Speech* speech = new Speech(&world, &actor, 6, newLines);
            Dialogue* dialogue = new Dialogue(&world, &actor, 6 );
            currentAbility = dialogue;
            actor.abilityQ.push_back(dialogue);
            stepBegun = true;
        }
        if (step == 0 && currentAbility->on == false) {
            step++;
            stepBegun = false;
        }
            
        if (step == 1 && !stepBegun) {
            FallingLetters* letters = new FallingLetters(&world, &actor, 15);
            letters->setTarget(biggestTarget);
            actor.abilityQ.push_back(letters);
            currentAbility = letters;
            stepBegun = true;
        }
            if (step == 1 && currentAbility->on == false) {
                step++;
                stepBegun = false;
            }
            break;
    }
}
