//
//  ScriptWakeUp.cpp
//  Pong
//
//  Created by Joseph Gu on 11/13/21.
//

#include "ScriptWakeUp.hpp"
#include "NameComponent.hpp"
#include "CombatComponent.hpp"
#include "World.hpp"
#include "Fish.hpp" 
#include "LifeComponent.hpp"

std::vector<std::string> myCrew = {
    "Floro", "Moonbell" 
};
 
ScriptWakeUp::ScriptWakeUp(World* world) : Script(world, myCrew) {
      
}

void ScriptWakeUp::act() {
    actors.at("Moonbell")->turnTowards(actors.at("Floro").get());

     
    if (glm::length(actors.at("Floro")->getPos() - posVec) > 10.0) {
            actors.at("Floro")->getComponent<LifeComponent>()->incStatValue(-1.0f, STAT_LIFE);
        if (stopWatch.getTime() > 3.0f) {
            actors.at("Floro")->getComponent<NameComponent>()->speak("So cold...", 1.0);
            stopWatch.resetTime();
        }
    } 
        
 
    if (glm::length(actors.at("Floro")->getPos() - posVec) > 10.0) {
            actors.at("Floro")->getComponent<LifeComponent>()->incStatValue(-1.0f, STAT_LIFE);
        if (stopWatch.getTime() > 3.0f) {
            actors.at("Floro")->getComponent<NameComponent>()->speak("So cold...", 1.0);
            stopWatch.resetTime();
        } 
    }
    switch (step) {    
        case 0: { 
            snow = pf.makeParticles(PE_SNOW, actors.at("Moonbell")->getPos()+glm::vec3(0,5,0));
            world->insert<ParticleSystem>(snow);
            actors.at("Moonbell")->getComponent<NameComponent>()->speak("Floro...? You're finally awake.", 3.0);
            step++; 
            break;  
        }
        case 1: { 
            waitFor(4.0);
            break;
        }
        case 2: {
            if (!isWaiting()) {
            actors.at("Floro")->getComponent<NameComponent>()->speak("Ahhh... AHH...", 1.0);
            }
            waitFor(1.0);
            break;
        }
        case 3: {
            actors.at("Floro")->getComponent<NameComponent>()->speak("What... Where... Who...", 1.0);
            step++;
            break;
        }
        case 4: {
            waitFor(2.0);
            actors.at("Floro")->jump(); 
            break;
        }
        case 5: {
            actors.at("Moonbell")->getComponent<NameComponent>()->speak("Floro, just be patient. \nIt's me, Moonbell.", 3.0);
            step++;
            break;
        }
        case 6: {
            waitFor(4.0);
            break;
        }
        case 7: {
            actors.at("Moonbell")->getComponent<NameComponent>()->speak("But you don't remember me anymore, \ndo you...", 3.0);
            step++;
            break;
        }
        case 8: {
            waitFor(4.0);
            break;
        }
        case 9: {
            actors.at("Floro")->getComponent<NameComponent>()->speak("Remember? What...", 3.0);
            step++;
            break;
        }
        case 10: {
            waitFor(4.0);
            break;
        }
        case 11: {
            actors.at("Moonbell")->getComponent<NameComponent>()->speak("...", 3.0);
            step++;
            break;
        }
        case 12: {
            waitFor(3.0);
            break;
        }
        case 13: {
            if (!isWaiting()) {
            actors.at("Moonbell")->getComponent<NameComponent>()->speak("You've lost your memory.", 1.0);
            }
            waitFor(2.0);
            break;
        }
        case 14: {
            if (!isWaiting()) {
            actors.at("Moonbell")->getComponent<NameComponent>()->speak("I'm.. your friend.", 1.5);
            }
            waitFor(2.0);
            break;
        }
        case 15: {
            if (!isWaiting()) {
            actors.at("Moonbell")->getComponent<NameComponent>()->speak("Don't go! Listen...", 1.5);
            }
            waitFor(2.0);
            break;
        }
        case 16: {
            if (!isWaiting()) {
            actors.at("Moonbell")->getComponent<NameComponent>()->speak("You're still weak, you won't make it out there. \nYou have to trust me.", 2.0);
            }
            waitFor(3.0);
            break;
        }
        case 17: {
            if (!isWaiting()) {
            actors.at("Moonbell")->getComponent<NameComponent>()->speak("Don't try to leave me. Or try. \nGo on, try it.", 2.0);
            }
            waitFor(3.0);
            break; 
        } 
        case 18: {
            if (actors.at("Floro")->getComponent<LifeComponent>()->getStat(STAT_LIFE).value <= 0) {
                actors.at("Floro")->setPos(actors.at("Moonbell")->getPos()+glm::vec3(1,0,5));
                actors.at("Moonbell")->getComponent<NameComponent>()->speak("Don't try that again", 2.0);
                actors.at("Floro")->getComponent<LifeComponent>()->incStatValue(50, STAT_LIFE);
                step++;
            } 
            break;
        }
 
                  
        default:
            break; 
    }
}
 
