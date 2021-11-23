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

std::vector<std::string> myCrew = {
    "Floro", "Moonbell"
};
 
ScriptWakeUp::ScriptWakeUp(World* world) : Script(world, myCrew) {
      
}

void ScriptWakeUp::act() {
    actors.at("Moonbell")->turnTowards(actors.at("Floro").get());
    
    switch (step) {
        case 0: {
            snow = pf.makeParticles(PE_SNOW, actors.at("Moonbell")->getPos()+glm::vec3(0,5,0));
            world->insert<ParticleSystem>(snow);
            actors.at("Moonbell")->getComponent<NameComponent>()->speak("Floro...?", 3.0);
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
            actors.at("Moonbell")->getComponent<NameComponent>()->speak("Floro! Don't worry, it's me, Moonbell.", 3.0);
            step++;
            break;
        }
        case 6: {
            waitFor(4.0);
            break;
        }
        case 7: {
            actors.at("Moonbell")->getComponent<NameComponent>()->speak("But you wouldn't remember anymore, would you...", 3.0);
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
            waitFor(4.0);
            break;
        }
        case 13: {
            actors.at("Floro")->getComponent<NameComponent>()->speak("Please... help me.. I'm a bit lost...", 3.0);
            step++;
            break; 
        }
        default:
            break;
    }
}
 
