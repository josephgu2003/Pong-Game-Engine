//
//  ScriptNightFlowers.cpp
//  Pong
//
//  Created by Joseph Gu on 1/16/22.
//

#include "ScriptNightFlowers.hpp"
#include "World.hpp"
#include "NotificationSystem.hpp"

std::vector<std::string> fitnCrew = {
    "Floro", "Moonbell"
};

std::vector<std::string> fitnprereqs = {"Snowclaw"};
  
ScriptNightFlowers::ScriptNightFlowers(World* world, bool completed) : Script(world, fitnCrew, 30.0f, completed, "Flowers in the Night", fitnprereqs) {
     
}

void ScriptNightFlowers::act() {
    Actor* moonbell = getActorNamed("Moonbell");
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
            break;
        }
        case 3: {
            if (!isWaiting()) {
            speak("Moonbell", "The wolf was right. The past is broken and cannot be returned to.", 3.0f);
            }
            waitFor(6.0f);
            break;
        }
        case 4: {
            if (!isWaiting()) {
            speak("Floro", "You can put down your sword and turn around right now. How is life different?", 3.0f);
            }
            waitFor(6.0f);
            break;
        };
        case 5: {
            if (!isWaiting()) {
            speak("Moonbell", "Can I? What if I can't?", 3.0f);
            }
            waitFor(6.0f);
            break;
        }
        case 6: {
            if (!isWaiting()) {
            speak("Moonbell", "You are a nobody. You can have dreams, I can't.", 3.0f);
            }
            waitFor(6.0f);
            break; 
        }
        case 7: {
            if (!isWaiting()) {
            speak("Floro", "If you aren't a nobody, who are you?", 3.0f);
            }
            waitFor(6.0f);
            break;
        }
        case 8: {
            if (!isWaiting()) {
            speak("Moonbell", "The truth is, our background is no longer so different", 3.0f);
            }
            waitFor(2.0f);
            break;
        }
        case 9: {
            
        }
    }
}
