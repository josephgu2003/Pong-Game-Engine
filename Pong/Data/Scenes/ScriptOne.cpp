//
//  ScriptOne.cpp
//  Pong
//
//  Created by Joseph Gu on 7/9/21.
//

#include "ScriptOne.hpp"
#include "Game.hpp"
#include "Dialogue.hpp"
#include "FallingLetters.hpp"
#include "Speech.hpp"
#include "CombatComponent.hpp"
#include "AnimComponent.hpp"

void ScriptOne::tick() {
    switch (state) {
        case STATE_PASSIVE: {

            if ((static_pointer_cast<CombatComponent>(ball->getComp(COMBAT)))->hasTarget()) {
                state = STATE_AGGRESSIVE;
            }

        }; 
            
        case STATE_AGGRESSIVE:

            if ((static_pointer_cast<CombatComponent>(ball->getComp(COMBAT)))->hasTarget()) {
                ball->turnTowards(glm::cross(glm::vec3(0.0,1.0,0.0), ball->getPos()-pHero->getPos()));
                ball->translatePos(0.04f*glm::normalize(glm::cross(glm::vec3(0.0,1.0,0.0),  ball->getPos()-pHero->getPos())));
    
              //  if (ballDir == STATE_TRACK)
     //   ball->turnTowards(static_pointer_cast<CombatComponent>(ball->getComp(COMBAT))->getBigTarget()->getPos()-ball->getPos());
           //     if (ballDir == STATE_OPPOSITE)
     //   ball->turnTowards(horizonDir); 
                  
        if (step == 0) {
            currentAbility = std::make_shared<Dialogue>(world, ball, 6.0, 0);
            (static_pointer_cast<CombatComponent>(ball->getComp(COMBAT)))->newAbility(currentAbility);
            step++;
            static_cast<AnimComponent*>(ball->getComp(ANIM).get())->playAnim("Take 001");
            return; 
        }  
                 
        if (step == 1) {
     
            if (static_cast<Dialogue*>(currentAbility.get())->getActiveText() == "Figure: Look to the horizon.") { 
                ballDir = STATE_OPPOSITE;  
                horizonDir =  ball->getPos()-static_pointer_cast<CombatComponent>(ball->getComp(COMBAT))->getBigTarget()->getPos();
            }
            if (currentAbility->on == false) {
                
                ballDir = STATE_TRACK;
                step++;
                currentAbility.reset();
            currentAbility = std::make_shared<FallingLetters>(world, ball, 10.0);
            currentAbility->setTarget(static_pointer_cast<CombatComponent>(ball->getComp(COMBAT))->getBigTarget());
                static_pointer_cast<CombatComponent>(ball->getComp(COMBAT))->newAbility(currentAbility);
                static_pointer_cast<CombatComponent>(static_pointer_cast<CombatComponent>(ball->getComp(COMBAT))->getBigTarget()->getComp(COMBAT))->affect(currentAbility);
           }
            return;
        } 
                if (step == 2) {
                    timer += (float)glfwGetTime();
                    if (timer > 2.0) {
                        timer = 0;
                        step++;
                        currentAbility2.reset();
                        std::vector<std::string> strings = {"'I've made up my mind. I'm going.'"};
                        currentAbility2 = std::make_shared<Speech>(world, (pHero), 2.0,strings);
                        static_pointer_cast<CombatComponent>(pHero->getComp(COMBAT))->newAbility(currentAbility2);
                    }
                    return;
                }
                
                if (step == 3) {
                    if (currentAbility2->on == false ) {
                        step++;
                        currentAbility2.reset();
                        std::vector<std::string> strings = {"Figure: Oh? I'd like to see you try."};

                        currentAbility2 = std::make_shared<Speech>(world, (ball), 2.0,strings);
                        static_pointer_cast<CombatComponent>(ball->getComp(COMBAT))->newAbility(currentAbility2);
                    }
                    return;
                }
    
                if (step == 4) {
                    if (currentAbility->on == false && currentAbility2->on == false ) {
                        step++;
                        currentAbility.reset();
                        std::vector<std::string> strings = {"'If fish can fly, I can too.'", "'Into a brighter world.'"};
                        
                        currentAbility = std::make_shared<Speech>(world, (pHero), 3.0,strings);
                        static_pointer_cast<CombatComponent>(pHero->getComp(COMBAT))->newAbility(currentAbility);
                    }
                    return;
                }
                
            break;

            }
    }
}

void ScriptOne::init(Game* game) {
    pHero = static_cast<PlayerHero*>(game->getNumberable(2));
    ball = static_cast<Ball*>(game->getNumberable(1));
    world = static_cast<World*>(game->getNumberable(0));
}
