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
#include "ChargedSlash.hpp"
#include "NotificationSystem.hpp"
#include "World.hpp"
#include "LifeComponent.hpp"
#include "DirlightTransition.hpp"

std::vector<std::string> myCrew = {
    "Floro", "Moonbell"
};

std::vector<std::string> prereqs = {"FloroWakes"};
  
ScriptWolfBoyOne::ScriptWolfBoyOne(World* world, bool completed) : Script(world, myCrew, 30.0f, completed, "Snowclaw", prereqs) {
     
}

void ScriptWolfBoyOne::act() {
    Actor* Snowclaw = getActorNamed("Snowclaw");
    Actor* Floro = getActorNamed("Floro");
    Actor* Moonbell = getActorNamed("Moonbell");
    switch (step) {
        case 0: {
            if (!isWaiting()) { 
                Moonbell->addComponent<CombatComponent>(*Moonbell);
                MyActorFactory maf;
                auto Snowclaw = maf.makeActor(ACTOR_SCARF_CHAR);
                newActor("Snowclaw", Snowclaw);
                Actor* sc = getActorNamed("Snowclaw");
                sc->setPos(glm::vec3(84,20,15));
                sc->getComponent<LifeComponent>()->init(100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f);
                sc->addComponent<CombatComponent>(*sc);
                world->getComponent<NotificationSystem>()->newNotification("The Wolf", 5.0f);
            }
            waitFor(2.0f); 
            break;
        }
            
        case 1: {
            doAndWaitFor([&] () {speak("Floro", "I can hardly see anymore...", 2.0f);}, 5.0f);
            break;
        }
        case 2: { 
            if (Floro->getDistanceTo(Snowclaw) > 7.0) {
                Snowclaw->orientYawTo(Floro);
                Snowclaw->posDir(0.1);
            } else {
                step++;
            }
            getActorNamed("Moonbell")->orientYawTo(getActorNamed("Snowclaw"));
            break;
        }
        
        case 3: {
            glm::vec3 postobe = getActorNamed("Floro")->getPos()-getActorNamed("Snowclaw")->getDir();
            if (getActorNamed("Moonbell")->getDistanceTo(postobe) > 2.0) {
                getActorNamed("Moonbell")->translatePos(0.1f*glm::normalize(postobe-getActorNamed("Moonbell")->getPos()));
            }
            waitFor(4.0);
            getActorNamed("Moonbell")->orientYawTo(getActorNamed("Snowclaw"));
            break;
        }
        case 4: {
            if (!isWaiting()) {
                speak("Moonbell", "If your level were any lower you'd be blind...", 3.0);
            }
            waitFor(3.0);
            getActorNamed("Moonbell")->orientYawTo(getActorNamed("Snowclaw"));
            break;
        }
        case 5: {
            if (!isWaiting()) {
                speak("Snowclaw", "I think you know why I am here. I have heard the news.", 4.0);
            }
            waitFor(2.0);
            getActorNamed("Moonbell")->orientYawTo(getActorNamed("Snowclaw"));
            break; 
        }
        case 6: {
            std::vector<std::string> lines = {"The news are wrong. False rumors."};
            std::vector<float> duration = {4.0f};
            makeSpeech("Moonbell", lines, duration);
            getActorNamed("Moonbell")->orientYawTo(getActorNamed("Snowclaw"));
            step++;
            break; 
        }
        case 7: {if (noActiveSpeech()) step++; break;}
        case 8: {
            std::vector<std::string> lines = {"No one commands me.",
                "I am the wolf who hunts alone.", "But even I know that it's for the greater good.",
                "I have always doubted you even before." 
            };
            std::vector<float> duration = {3.0f, 3.0f, 4.0f, 4.0f};
            makeSpeech("Snowclaw", lines, duration);
            getActorNamed("Moonbell")->orientYawTo(getActorNamed("Snowclaw"));
            
            DirectionalLight             dl2(glm::vec3(0.01,0.01,0.01),glm::vec3(0.02,0.02,0.02),glm::vec3(0.0,0.0,0.0),glm::vec3(-1,-1,0));
            world->addComponent<DirlightTransition>(*world, 4.0f, dl2);
            
            step++;
            break;
        }
        case 9: {
            if(noActiveSpeech() || isWaiting()) {  // first time in is when speech ends, second time is waiting
                waitFor(3.0f);
            } 
            break;
        }
            
        case 10: {
            doAndWaitFor([&] () {getActorNamed("Snowclaw")->getComponent<AnimComponent>()->playAnim("DrawWeapon", 20,40);}, 1.0f);
            break;
        }; 
        case 11: {
            if (!isWaiting()) {
            std::vector<std::string> lines = {"I'm no monster.", "You, please fight this bastard for me.", "Eagles can't fight the battles of rabbits, so it's not proper for me to take this.", "You have a light in your pocket, set it free."};
            std::vector<float> durations = {2.5f, 2.5f, 3.5f, 3.5f};
            makeSpeech("Moonbell", lines, durations);
            } 
            waitFor(6.0f); 
            getActorNamed("Snowclaw")->orientYawTo(getActorNamed("Floro"));
            break;
        }
        case 12: {
            if(getActorNamed("Floro")->getComponent<LifeComponent>()->getStat(STAT_LIFE).value <= 0.0f && !isWaiting()) {
                getActorNamed("Moonbell")->getComponent<CombatComponent>()->newAbility<FallingLetters>(world, getActorNamed("Moonbell"), 2.0f);
                step += 2; 
                break;
            }
            if(getActorNamed("Snowclaw")->getComponent<LifeComponent>()->getStat(STAT_LIFE).value <= 0.0f && !isWaiting()) {
                step += 2;
                break;
            } 
            if (!isWaiting()) { 
            getActorNamed("Snowclaw")->getComponent<CombatComponent>()->newAbility<ChargedSlash>(world, getActorNamed("Snowclaw"), 2.0f);
            }
            waitFor(2.0f);
            getActorNamed("Snowclaw")->orientYawTo(getActorNamed("Floro"));
            break;
        } 
        case 13: {
            step--;
            break; 
        }  
        case 14: {
            doAndWaitFor([&] () {
                DirectionalLight            dl2(glm::vec3(0.1,0.09,0.15),glm::vec3(0.1,0.1,0.1),glm::vec3(0.8,0.8,0.8),glm::vec3(-1,-1,0));
                world->addComponent<DirlightTransition>(*world, 10.0f, dl2);
                getActorNamed("Snowclaw")->getComponent<AnimComponent>()->playAnim("Lying", true);},5.0f);
            break;
        }
        case 15: {
            speak("Moonbell", "Do you regret it now? You should've stayed away! Why?", 2.0f);
            getActorNamed("Moonbell")->orientYawTo(getActorNamed("Snowclaw"));
            getActorNamed("Moonbell")->getComponent<AnimComponent>()->playAnim("DrawWeapon", 20,40);
            step++; 
            break;
        }
        case 16: {
            world->getComponent<NotificationSystem>()->newNotification("Intervene, or stand and watch.", 4.0f);
            step++;
            break;
        }
        case 17: {
            if (getActorNamed("Floro")->getDistanceTo(getActorNamed("Snowclaw")) < 1.5f) {
                step += 2;
            }
            if (getActorNamed("Moonbell")->getDistanceTo(getActorNamed("Snowclaw")) < 2.0f) {
                step++;
            }
            getActorNamed("Moonbell")->posDir(0.01);
            break;
        }
        case 18: {
            speak("Moonbell", "Die.", 3.0f);
            getActorNamed("Moonbell")->getComponent<CombatComponent>()->newAbility<FallingLetters>(world, getActorNamed("Moonbell"), 2.0f);
            step+=2;
            break;
        }
            
        case 19: {
            std::vector<std::string> lines = {"Get out of my way.", "..."};
            std::vector<float> durations = {4.0f,8.0f,4.0f};
            makeSpeech("Moonbell", lines, durations);
            step += 2;
            break;
        }
        case 20: {
            speak("Moonbell", "Let's go.", 3.0f);
            getActorNamed("Moonbell")->getComponent<AnimComponent>()->playDefault();
            endScene();   
            break;
        }
            
        case 21: {
            if (noActiveSpeech()) {
                std::vector<std::string> lines = {"He's hurt. Who are you? Why so cruel?", "I'm the one who fought him. His outcome is decided by me."};
                std::vector<float> durations = {4.0f,4.0f};
                makeSpeech("Floro", lines, durations);
                step++;  
                break;
            } 
            break;  
        } 
        case 22: {
            if (noActiveSpeech()) {
                std::vector<std::string> lines = {"Eagles shouldn't meddle in the affairs of rabbits.", "Let's leave, then."};
                std::vector<float> durations = {4.0f,4.0f};
                makeSpeech("Moonbell", lines, durations);
                getActorNamed("Moonbell")->getComponent<AnimComponent>()->playDefault();
                step++;
                break;
            }
            break;
        }
        case 23: {
            endScene();
            break;
        }
       default:
           break;
    }
}
