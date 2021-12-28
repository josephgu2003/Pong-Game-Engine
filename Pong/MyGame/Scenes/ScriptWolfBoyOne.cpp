//
//  ScriptWolfBoyOne.cpp
//  Pong
//
//  Created by Joseph Gu on 12/25/21.
//

#include "ScriptWolfBoyOne.hpp"
#include "AnimComponent.hpp"
#include "MyActorFactory.hpp"
#include "CombatComponent.hpp" 
#include "FallingLetters.hpp"
#include "SwordWorld.hpp"

std::vector<std::string> myCrew = {
    "Floro", "Moonbell"
};

std::vector<std::string> prereqs = {"FloroWakes"};
  
ScriptWolfBoyOne::ScriptWolfBoyOne(World* world, bool completed) : Script(world, myCrew, 30.0f, completed, "Snowclaw", prereqs) {
     
}

void ScriptWolfBoyOne::act() {
    switch (step) {
        case 0: {
            if (!isWaiting()) {
                MyActorFactory maf;
                auto Snowclaw = maf.makeActor(ACTOR_SCARF_CHAR);
                newActor("Snowclaw", Snowclaw);
                getActorNamed("Snowclaw")->setPos(glm::vec3(60,44,-16));
            }
            waitFor(2.0f);
            break;
        }
        case 1: {
            if (!isWaiting()) speak("Floro", "Why are we stopping? I want to go home.", 4.0f);
            waitFor(5.0f);
            break;
        }
        case 2: {
            if (!isWaiting()) speak("Moonbell", "You have no home. Stop, a wolf is hunting our scent.", 5.0f);
            waitFor(5.0f);
            break;
        }
        case 3: {
            if (getActorNamed("Moonbell")->getDistanceTo(getActorNamed("Snowclaw")) > 5.0) {
                getActorNamed("Snowclaw")->turnTowards(getActorNamed("Moonbell"));
                getActorNamed("Snowclaw")->posDir(0.1);
            } else {
                step++;
            }
            getActorNamed("Moonbell")->turnTowards(getActorNamed("Snowclaw"));
            break;
        }
        
        case 4: {
            if (!isWaiting()) {
                speak("Snowclaw", "The cold has never bothered you, witch, but the inheritor seems to be a bit chilly.", 5.0);
            }
            waitFor(5.0);
            getActorNamed("Moonbell")->turnTowards(getActorNamed("Snowclaw"));
            break;
        }
        case 5: {
            if (!isWaiting()) {
                speak("Moonbell", "Leave us be. He remembers nothing.", 2.0);
            }
            waitFor(3.0);
            getActorNamed("Moonbell")->turnTowards(getActorNamed("Snowclaw"));
            break;
        }
        case 6: {
            if (!isWaiting()) {
                speak("Floro", "Who are you?", 2.0);
            }
            waitFor(3.0);
            getActorNamed("Moonbell")->turnTowards(getActorNamed("Snowclaw"));
            break; 
        }
        case 7: {
                std::vector<std::string> lines = {"A lone wolf, who grew up in the cold.", "He allies with no one, but administers justice as he sees fit."};
                std::vector<float> duration = {5.0f,5.0f};
                makeSpeech("Moonbell", lines, duration);
            getActorNamed("Moonbell")->turnTowards(getActorNamed("Snowclaw"));
            step++;
            break;
        }
        case 8: {if (noActiveSpeech()) step++; break;}
        case 9: {
            std::vector<std::string> lines = {"You only need know one thing", "You two are a menace, and I am here to hunt."};
            std::vector<float> duration = {5.0f,5.0f};
            makeSpeech("Snowclaw", lines, duration);
            getActorNamed("Moonbell")->turnTowards(getActorNamed("Snowclaw"));
            step++;
            break;
        }
        case 10: {
            if(noActiveSpeech() || isWaiting()) {  // first time in is when speech ends, second time is waiting
                if (!isWaiting()) {
                getActorNamed("Snowclaw")->getComponent<CombatComponent>()->newAbility<FallingLetters>(world, getActorNamed("Snowclaw"), 100.0f, getActorRefNamed("Floro"));
                getActorNamed("Snowclaw")->getComponent<AnimComponent>()->playAnim("DrawWeapon", false);
                }
                waitFor(2.0f);
            } 
            break;
        }
            
        case 11: {
            if (!isWaiting()) {
            std::vector<std::string> lines = {"You would use such a strong Runic attack on a cripple?", "You are as ruthless as me. Floro, I can't help, but the fish charm can.", "Break the attack, and I'll break him."};
            std::vector<float> durations = {5.0f, 3.5f, 4.0f,};
            makeSpeech("Moonbell", lines, durations);
            }
            waitFor(9.0f);
            break;
        }
        case 12: {
            getActorNamed("Moonbell")->getComponent<CombatComponent>()->newAbility<SwordWorld>(world, getActorNamed("Moonbell"), 10.0f);
            step++;
            break;
        }
       default: 
           break;
    }
}
