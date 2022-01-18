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
                MyActorFactory maf;
                auto Snowclaw = maf.makeActor(ACTOR_SCARF_CHAR);
                newActor("Snowclaw", Snowclaw);
                getActorNamed("Snowclaw")->setPos(glm::vec3(60,44,-16));
                getActorNamed("Snowclaw")->getComponent<LifeComponent>()->init(100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f);
                world->getComponent<NotificationSystem>()->newNotification("The Wolf", 5.0f);
            }
            waitFor(2.0f);
            break;
        }
        case 1: {
            if (!isWaiting()) speak("Floro", "What's that sound?", 2.0f);
            waitFor(5.0f);
            break;
        }
        case 2: {
      //      if (!isWaiting()) speak("Moonbell", "The wolf.", 2.0f);
      //      waitFor(2.0f);
            step++;
            break;
        }
        case 3: {
            if (getActorNamed("Floro")->getDistanceTo(getActorNamed("Snowclaw")) > 7.0) {
                getActorNamed("Snowclaw")->orientYawTo(getActorNamed("Floro"));
                getActorNamed("Snowclaw")->posDir(0.1);
            } else {
                step++;
            }
            getActorNamed("Moonbell")->orientYawTo(getActorNamed("Snowclaw"));
            break;
        }
        
        case 4: {
            glm::vec3 postobe = getActorNamed("Floro")->getPos()-getActorNamed("Snowclaw")->getDir();
            if (getActorNamed("Moonbell")->getDistanceTo(postobe) > 2.0) {
                getActorNamed("Moonbell")->translatePos(0.1f*glm::normalize(postobe-getActorNamed("Moonbell")->getPos()));
            }
            waitFor(4.0);
            getActorNamed("Moonbell")->orientYawTo(getActorNamed("Snowclaw"));
            break;
        }
        case 5: {
            if (!isWaiting()) {
                speak("Moonbell", "Leave us be. He remembers nothing.", 2.0);
            }
            waitFor(3.0);
            getActorNamed("Moonbell")->orientYawTo(getActorNamed("Snowclaw"));
            break;
        }
        case 6: {
            if (!isWaiting()) {
                speak("Floro", "Who are you?", 1.5);
            }
            waitFor(2.0);
            getActorNamed("Moonbell")->orientYawTo(getActorNamed("Snowclaw"));
            break; 
        }
        case 7: {
                std::vector<std::string> lines = {"A lone wolf, who grew up in the cold, and a self-proclaimed hero.", "Get out of our way, you are not our target."};
                std::vector<float> duration = {2.0f,3.0f};
                makeSpeech("Moonbell", lines, duration);
            getActorNamed("Moonbell")->orientYawTo(getActorNamed("Snowclaw"));
            step++;
            break; 
        }
        case 8: {if (noActiveSpeech()) step++; break;}
        case 9: {
            std::vector<std::string> lines = {"I only want to end this tragic story before it takes more lives.", "Though, you, if you survive, run away.", "The past is broken and cannot be returned to."};
            std::vector<float> duration = {4.0f,3.0f,3.0f};
            makeSpeech("Snowclaw", lines, duration);
            getActorNamed("Moonbell")->orientYawTo(getActorNamed("Snowclaw"));
            step++;
            break;
        }
        case 10: {
            if(noActiveSpeech() || isWaiting()) {  // first time in is when speech ends, second time is waiting
                if (!isWaiting()) {
                    DirectionalLight            dl2(glm::vec3(0.1,0.09,0.15),glm::vec3(0.1,0.1,0.1),glm::vec3(0.8,0.8,0.8),glm::vec3(-1,-1,0));
                    std::vector<std::string> sky1 = { 
                        "Resources/Skybox/NightStars/BlueNebular_left.jpg",
                        "Resources/Skybox/NightStars/BlueNebular_right.jpg",
                        "Resources/Skybox/NightStars/BlueNebular_top.jpg",
                        "Resources/Skybox/NightStars/BlueNebular_bottom.jpg",
                        "Resources/Skybox/NightStars/BlueNebular_front.jpg",
                        "Resources/Skybox/NightStars/BlueNebular_back.jpg"
                    };
                    world->setWeather(dl2, 0.01, 1.0, glm::vec3(0.35,0.33,0.4), sky1);
                }
                waitFor(3.0f);  
            } 
            break;
        }
            
        case 11: {
            if (!isWaiting()) {
              //  getActorNamed("Snowclaw")->getComponent<CombatComponent>()->newAbility<FallingLetters>(world, getActorNamed("Snowclaw"), 100.0f, getActorRefNamed("Floro"));
                getActorNamed("Snowclaw")->getComponent<AnimComponent>()->playAnim("DrawWeapon", 20,40);
            }
            waitFor(1.0f);
            break;
        };
        case 12: {
      //      speak("Floro", "Ouch!", 1.0f);
            step++; 
            break;
        };
        case 13: {
            if (!isWaiting()) {
            std::vector<std::string> lines = {"Ignore him. He is only trying to distract you.", "You have a Charm in your pocket.", "Light your way and survive."};
            std::vector<float> durations = {2.5f, 3.5f, 2.0f};
            makeSpeech("Moonbell", lines, durations);
            } 
            waitFor(6.0f);
            getActorNamed("Snowclaw")->orientYawTo(getActorNamed("Floro"));
            break;
        }
        case 14: {
            if (getActorNamed("Snowclaw")->getComponent<LifeComponent>()->getStat(STAT_LIFE).value <= 0.0f && !isWaiting()) {
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
        case 15: {
            step--;
            break; 
        }  
        case 16: {
            if (!isWaiting()) { 
                getActorNamed("Snowclaw")->getComponent<AnimComponent>()->playAnim("Lying", true);
                speak("Snowclaw", "Ah... You shouldn't do this...", 5.0f);
            }
            waitFor(5.0f);
            break;
        }
        case 17: { 
            speak("Moonbell", "Enough of your lies. Let's end this.", 2.0f);
            getActorNamed("Moonbell")->orientYawTo(getActorNamed("Snowclaw"));
            getActorNamed("Moonbell")->getComponent<AnimComponent>()->playAnim("DrawWeapon", 20,40);
            step++;
            break;
        }
        case 18: {
            world->getComponent<NotificationSystem>()->newNotification("Choose: Intervene, or let Snowclaw die.", 4.0f);
            step++;
            break;
        }
        case 19: {
            if (getActorNamed("Floro")->getDistanceTo(getActorNamed("Snowclaw")) < 1.5f) {
                step += 2;
                break;
            }
            if (getActorNamed("Moonbell")->getDistanceTo(getActorNamed("Snowclaw")) < 2.0f) {
                step ++;
                break;
            }
            getActorNamed("Moonbell")->posDir(0.01);
            break;
        }  
        case 20: { 
            speak("Moonbell", "Die.", 3.0f);
            getActorNamed("Moonbell")->getComponent<CombatComponent>()->newAbility<FallingLetters>(world, getActorNamed("Moonbell"), 2.0f);
            step+=2;
            break;
        }
        case 21: {
            std::vector<std::string> lines = {"Get out of my way.", "...", "You are so stupid. Let's go."};
            std::vector<float> durations = {4.0f,8.0f,4.0f};
            makeSpeech("Moonbell", lines, durations);
            step+=2;
            break;
        }
        case 22: {
            speak("Moonbell", "Let's go.", 3.0f);
            getActorNamed("Moonbell")->getComponent<AnimComponent>()->playDefault();
            endScene();   
            break;
        }
        case 23: {
            if (noActiveSpeech()) {
                getActorNamed("Moonbell")->getComponent<AnimComponent>()->playDefault();
                endScene();
            }
            break;
        }
       default: 
           break;
    }
}
