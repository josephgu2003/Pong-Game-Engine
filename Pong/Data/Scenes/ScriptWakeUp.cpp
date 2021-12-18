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
#include "PGraphicsComponent.hpp"
#include "FallingLetters.hpp"
#include "SwordWorld.hpp"

std::vector<std::string> myCrew = {
    "Floro", "Moonbell" 
};
  
ScriptWakeUp::ScriptWakeUp(World* world) : Script(world, myCrew, 50.0f) {
      
}
 
void ScriptWakeUp::act() {
    posVec = getActorNamed("Moonbell")->getPos();
    
    if (step < 21) {
    getActorNamed("Moonbell")->turnTowards(getActorNamed("Floro"));
    }

    if (glm::length(getActorNamed("Floro")->getPos() - getActorNamed("Moonbell")->getPos()) > 10.0) {
            getActorNamed("Floro")->getComponent<LifeComponent>()->incStatValue(-1.0f, STAT_LIFE);
        if (stopWatch.getTime() > 3.0f) {
            getActorNamed("Floro")->getComponent<NameComponent>()->speak("So cold...", 1.0);
            stopWatch.resetTime();
        }
    } 
         
    switch (step) {    
        case 0: { 
            if (!isWaiting()) { 
                snow = pf.makeParticles(PE_SNOW, getActorNamed("Moonbell")->getPos()+glm::vec3(0,5,0));
                snow->getComponent<PGraphicsComponent>()->setColor(0.1, 0.1, 0.1);
                world->insert<ParticleSystem>(snow); 
            }
 
            waitFor(4.0);
            break;   
        }
        case 1: {
            if (!isWaiting()) {
                getActorNamed("Moonbell")->getComponent<NameComponent>()->speak("Is this where dreams go to die?", 3.0);
            }
            waitFor(4.0);
            break;
        }
        case 2: {
            if (!isWaiting()) {
                getActorNamed("Moonbell")->getComponent<NameComponent>()->speak("Do you regret? When you slept,\n what dreams could you remember?", 3.0);
            }
            waitFor(4.0);
            break; 
        } 
        case 3: {
            getActorNamed("Floro")->getComponent<NameComponent>()->speak("Who are you?", 2.0);
            step++;
            break;
        }
        case 4: {
            waitFor(3.0);
            getActorNamed("Floro")->jump();
            break;
        }
        case 5: {
            getActorNamed("Moonbell")->getComponent<NameComponent>()->speak("Feeling heroic now?", 3.0);
            step++;
            break;
        }
        case 6: {
            waitFor(3.0);
            break;
        }
        case 7: {
            getActorNamed("Moonbell")->getComponent<NameComponent>()->speak("You don't remember me anymore, \ndo you...", 3.0);
            step++;
            break;
        }
        case 8: {
            waitFor(4.0);
            break;
        }
        case 9: {
            getActorNamed("Floro")->getComponent<NameComponent>()->speak("Remember? What...", 3.0);
            step++;
            break;
        }
        case 10: {
            waitFor(4.0);
            break;
        }
        case 11: {
            getActorNamed("Moonbell")->getComponent<NameComponent>()->speak("...", 3.0);
            step++;
            break;
        }
        case 12: {
            waitFor(3.0);
            break;
        }
        case 13: {
            if (!isWaiting()) {
            getActorNamed("Moonbell")->getComponent<NameComponent>()->speak("So you've really lost your memory.", 2.0);
            }
            waitFor(3.0);
            break;
        }
        case 14: {
            if (!isWaiting()) {
            getActorNamed("Moonbell")->getComponent<NameComponent>()->speak("I am Moonbell.. your friend.", 2);
            }
            waitFor(2.5);
            break;
        }
        case 15: {
            if (!isWaiting()) {
            getActorNamed("Moonbell")->getComponent<NameComponent>()->speak("Don't go! Stay here and listen...", 2.5);
            }
            waitFor(3.0);
            break;
        }
        case 16: {
            if (!isWaiting()) {
            getActorNamed("Moonbell")->getComponent<NameComponent>()->speak("You're hurt, you won't make it out there. \nYou have to trust me.", 2.0);
            }
            waitFor(3.0);
            break;
        }
        case 17: {
            if (!isWaiting()) {
            getActorNamed("Moonbell")->getComponent<NameComponent>()->speak("Don't try to leave me. Or actually... try. \nGo on, try it.", 3.0);
            }
            waitFor(3.0);
            break; 
        }
        case 18: {
            if (getActorNamed("Floro")->getComponent<LifeComponent>()->getStat(STAT_LIFE).value <= 0) {
                getActorNamed("Floro")->setPos(getActorNamed("Moonbell")->getPos()+glm::vec3(1,0,5));
                getActorNamed("Moonbell")->getComponent<NameComponent>()->speak("Don't try that again", 2.0);
                getActorNamed("Floro")->getComponent<LifeComponent>()->incStatValue(50, STAT_LIFE);
                step++;
            } 
            break;
        }
        case 19: {
            waitFor(3.0);
            break;
        }
 
        case 20: {
            if (!isWaiting()) {
                getActorNamed("Moonbell")->getComponent<NameComponent>()->speak("We have important things to do. \nI want you to remember your past.", 2.0);
            }
            waitFor(3.0);
            break;
        }
        case 21: {
            if (!isWaiting()) {
                getActorNamed("Moonbell")->getComponent<NameComponent>()->speak("There are things that are your birthright. \nCome, try to keep up.", 2.0);
            }
            getActorNamed("Moonbell")->turnTowards(glm::vec3(74,44,-2)-getActorNamed("Moonbell")->getPos());
            waitFor(3.0);
            break;
        }
        case 22: {
            getActorNamed("Moonbell")->posDir(0.1);
            if (getActorNamed("Moonbell")->getDistanceTo(glm::vec3(74,44,-2)) < 0.5) {
                step++;
            } 
            break;
        }
        case 23: {
            if (!isWaiting()) { 
                getActorNamed("Moonbell")->getComponent<NameComponent>()->speak("Stop. Something is wrong.", 2.0);
                auto skar = maf.makeActor(ACTOR_KNIGHT);
                newActor("Skar", skar);
                getActorNamed("Skar")->setPos(glm::vec3(60,44,-16));
            }
            waitFor(3.0);
            break;
        } 
        case 24: {
            if (getActorNamed("Moonbell")->getDistanceTo(getActorNamed("Skar")) > 10.0) {
                getActorNamed("Skar")->turnTowards(getActorNamed("Moonbell"));
                getActorNamed("Skar")->posDir(0.1);
            } else {
                step++;
            }
            break;
        }
         
        case 25: {
            if (!isWaiting()) {
                getActorNamed("Skar")->getComponent<NameComponent>()->speak("I can't let you pass. The goals you \n seek will not bring peace to this world.", 2.0);
            } 
            waitFor(3.0);
            break;
        }
        case 26: {
            if (!isWaiting()) {
                getActorNamed("Moonbell")->getComponent<NameComponent>()->speak("No. We will bring peace to this world and for \n ourselves. We can have both.", 2.0);
            }
            waitFor(3.0);
            break;
        }
        case 27: {
            if (!isWaiting()) {
                getActorNamed("Skar")->getComponent<NameComponent>()->speak("I have decided. For the greater good, \n he will not live beyond tonight.", 2.0);
            }
            waitFor(3.0);
            break;
        }  
        case 28: {
            if (!isWaiting()) {
                std::shared_ptr<Ability> letters = std::make_shared<FallingLetters>(world,   getActorNamed("Skar"), 10.0);
                getActorNamed("Skar")->getComponent<CombatComponent>()->newAbility(letters);
            }
            waitFor(3.0);
            break;
        }
        case 29: {
            if (!isWaiting()) {
                std::shared_ptr<Ability> fish = std::make_shared<Fish>(world,   getActorNamed("Moonbell"), 10.0);

                getActorNamed("Moonbell")->getComponent<CombatComponent>()->newAbility(fish);
                getActorNamed("Moonbell")->getComponent<NameComponent>()->speak("Stay back, Floro. Take this \n chance to relearn some old tricks.", 2.0);
            }
            waitFor(3.0);
            break;
        }
        case 30: {
            if (!isWaiting()) {
                std::shared_ptr<Ability> fish = std::make_shared<SwordWorld>(world,   getActorNamed("Moonbell"), 10.0);

                getActorNamed("Moonbell")->getComponent<CombatComponent>()->newAbility(fish);
                getActorNamed("Moonbell")->getComponent<NameComponent>()->speak("50 swords, one for each second \n of my time you wasted, Skar.", 2.0);
            }
            waitFor(3.0);
            break;
        }
            
        default:
            break;
    }
}
  
