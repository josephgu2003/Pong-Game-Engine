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

            if (ball->getComponent<CombatComponent>()->hasTarget()) {
                state = STATE_AGGRESSIVE;
            }

        }; 
            
        case STATE_AGGRESSIVE:

            if (ball->getComponent<CombatComponent>()->hasTarget()) {
                ball->turnTowards(glm::cross(glm::vec3(0.0,1.0,0.0), ball->getPos()-pHero->getPos()));
                ball->translatePos(0.04f*glm::normalize(glm::cross(glm::vec3(0.0,1.0,0.0),  ball->getPos()-pHero->getPos())));
    
              //  if (ballDir == STATE_TRACK)
     //   ball->turnTowards(static_pointer_cast<CombatComponent>(ball->getComp(COMBAT))->getBigTarget()->getPos()-ball->getPos());
           //     if (ballDir == STATE_OPPOSITE)
     //   ball->turnTowards(horizonDir); 
                  
        if (step == 0) {
            currentAbility = std::make_shared<Dialogue>(&ball->getWorld(), ball, 6.0, 0);
            ball->getComponent<CombatComponent>()->newAbility(currentAbility);
            step++;
            return; 
        }  
                  
        if (step == 1) {
     
            if (static_cast<Dialogue*>(currentAbility.get())->getActiveText() == "Figure: Look to the horizon.") { 
                ballDir = STATE_OPPOSITE;  
                horizonDir =  ball->getPos()-ball->getComponent<CombatComponent>()->getBigTarget()->getPos();
            }
            if (currentAbility->on == false) {
                
                ballDir = STATE_TRACK;
                step++;
                currentAbility.reset();
            currentAbility = std::make_shared<FallingLetters>(&ball->getWorld(), ball, 10.0);
            currentAbility->setTarget(ball->getComponent<CombatComponent>()->getBigTarget());
           ball->getComponent<CombatComponent>()->newAbility(currentAbility);
            ball->getComponent<CombatComponent>()->getBigTarget()->getComponent<CombatComponent>()->affect(currentAbility);
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
                        currentAbility2 = std::make_shared<Speech>(&pHero->getWorld(), (pHero), 2.0,strings);
                       pHero->getComponent<CombatComponent>()->newAbility(currentAbility2);
                    }
                    return; 
                }
                
                if (step == 3) {
                    if (currentAbility2->on == false ) {
                        step++;
                        currentAbility2.reset();
                        std::vector<std::string> strings = {"Figure: Oh? I'd like to see you try."};

                        currentAbility2 = std::make_shared<Speech>(&ball->getWorld(), (ball), 2.0,strings);
                        ball->getComponent<CombatComponent>()->newAbility(currentAbility2);
                    }
                    return;
                }
    
                if (step == 4) {
                    if (currentAbility->on == false && currentAbility2->on == false ) {
                        step++;
                        currentAbility.reset();
                        std::vector<std::string> strings = {"'If fish can fly, I can too.'", "'Into a brighter world.'"};
                        
                        currentAbility = std::make_shared<Speech>(&pHero->getWorld(), (pHero), 3.0,strings);
                        pHero->getComponent<CombatComponent>()->newAbility(currentAbility);
                    }
                    return;
                }
                
            break;

            }
    }
}

void ScriptOne::init(Game* game) {
    //pHero = static_cast<PlayerHero*>(game->getNumberable(2));
 //   ball = static_cast<Ball*>(game->getNumberable(1));
    pHero = game->getWorld(0).getActorNamed("pHero0").get();
    ball = game->getWorld(0).getActorNamed("LightChar").get();
}
