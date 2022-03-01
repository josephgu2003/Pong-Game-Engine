//
//  ScriptNightFlowers.cpp
//  Pong
//
//  Created by Joseph Gu on 1/16/22.
//

#include "ScriptNightFlowers.hpp"
#include "World.hpp"
#include "GraphicsComponent.hpp"
#include "NotificationSystem.hpp"
#include "SurvivalMode.hpp"

std::vector<std::string> fitnCrew = {
    "Floro", "Moonbell"
};

std::vector<std::string> fitnprereqs = {"Snowclaw"};
  
ScriptNightFlowers::ScriptNightFlowers(World* world, bool completed) : Script(world, fitnCrew, 30.0f, completed, "Flowers in the Night", fitnprereqs) {
     
}

void ScriptNightFlowers::act() {
    Actor* moonbell = getActorNamed("Moonbell");
    Actor* floro = getActorNamed("Floro");
    setPos(moonbell->getPos());

    switch (step) {
        case 0: { 
            world->getComponent<NotificationSystem>()->newNotification("Flowers in the Night", 5.0f);
            moonbell->orientYawTo(glm::vec3(200.0f,20.0f,-40.0f));
            moonbell->posDir(0.02);
            step++;
            break;
        }
        case 1: {
            moonbell->posDir(0.02);
            waitFor(10.0f);
            break;
        }
        case 2: {
            if (!isWaiting()) {
            speak("Floro", "I think I know you.", 3.0f);
            }
            waitFor(6.0f);
            moonbell->posDir(0.02);
            break;
        }
        case 3: {
            if (!isWaiting()) {
            speak("Moonbell", "...", 3.0f);
            }
            waitFor(6.0f);
            moonbell->posDir(0.02);
            break;
        }
        case 4: {
            if (!isWaiting()) {
            speak("Floro", "Where did you find me again?", 3.0f);
            }
            waitFor(6.0f);
            moonbell->posDir(0.02);
            break;
        };
        case 5: {
            if (!isWaiting()) {
            speak("Moonbell", "I heard a loud noise and when I came, you were unconscious.", 3.0f);
            }
            waitFor(6.0f);
            moonbell->posDir(0.02);
            break;
        }
        case 6: {
            if (!isWaiting()) {
            speak("Moonbell", "What is your name?", 3.0f);
            }
            waitFor(6.0f);
            moonbell->posDir(0.02);
            break; 
        }
        case 7: {
            if (!isWaiting()) {
            speak("Floro", "I can't remember much, but I know my name. Floro.", 3.0f);
            }
            waitFor(6.0f);
            moonbell->posDir(0.02);
            break;
        }
        case 8: {
            if (!isWaiting()) {
            speak("Moonbell", "Nice to meet you. I'm Moonbell.", 3.0f);
            }
            waitFor(10.0f);
            moonbell->posDir(0.02);
            break;
        }
        case 9: {
            if (!isWaiting()) {
            floro->getComponent<GraphicsComponent>()->setUniform<float>("brightness", 6.0f);
            }
            waitFor(3.0f);
            moonbell->posDir(0.02);
            break;
        }
        case 10: {
            if (!isWaiting()) {
            speak("Floro", "I'm glowing?", 2.0f);
            }
            waitFor(4.0f);
            break;
        } 
        case 11: {
            if (!isWaiting()) {
                speak("Moonbell", "Good. You are close then.", 2.0f);
                floro->getComponent<GraphicsComponent>()->setUniform<float>("brightness", 1.0f);
            } 
            waitFor(4.0f);
            break;
        }
            
        case 12: {
            break;
        }
            
        case 100: {
            world->addComponent<SurvivalMode>(*world, 100.0f);
            world->getComponent<NotificationSystem>()->newNotification("The rest is experimental gameplay - survival mode, perhaps more interesting", 10.0f);
            step++; 
            break;
        }
    }
}
