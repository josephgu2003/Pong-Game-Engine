//
//  InTheWilderness.cpp
//  Pong
//
//  Created by Joseph Gu on 3/16/22.
//

#include "InTheWilderness.hpp"
#include "World.hpp"
#include "NotificationSystem.hpp"
#include "AnimComponent.hpp"
#include "PropFactory.hpp"
#include "PaintingSubject.hpp"
#include "ParticleFactory.hpp"
#include "Painter.hpp"
#include "FogTransition.hpp"
#include "SkyColorTransition.hpp"
#include "MovementController.hpp"
#include "LightComponent.hpp"
#include "DirlightTransition.hpp"
#include "MyActorFactory.hpp"
#include "NameComponent.hpp"
#include "CombatComponent.hpp"
#include "FallingLetters.hpp"

std::vector<std::string> inWildsCrew = {
    "Floro"
};
  
std::vector<std::string> inWildsReqs = {};

InTheWilderness::InTheWilderness(World* world, bool completed, InputHandler* ih, uiLayout* ui) : MyScript(world, inWildsCrew, 200.0f, completed, "In the Wilderness", inWildsReqs, ih, ui) {
    encounterFlower = false;
    encounterTree = false;
    
    floroRandomQuotes.push("Elder Brother did nothing wrong. But some people can't let the past go.");
    floroRandomQuotes.push("Elder Brother is not my real brother. So idiot people murmur and gossip.");
    floroRandomQuotes.push("Elder Brother used to be a Shorewalker until he was crippled in a duel. I wish I could be a Shorewalker too.");
       
    randomQuoteInterval.windGentle(40.0f);
}
  
void InTheWilderness::act() {
    Actor* floro = getActorNamed("Floro");
    AnimComponent* anim = floro->getComponent<AnimComponent>();
    Painter* painter = floro->getComponent<Painter>();
    std::string recentPainting = painter->getMostRecentPainting();
    
    if ((recentPainting != "") && (paintedSubjects.find(recentPainting) == paintedSubjects.end())) {
        paintedSubjects.insert(recentPainting);
        floroPauseSpeak.windGentle(4.0f); 
         
        if (recentPainting == "Lonely Flower") {
            floroNextLines.push("I could dig you up and bring you home, flower, but you wouldn't want that, would you?");
        } else if (recentPainting == "Resilient Tree") {
            floroNextLines.push("Weak people are lucky to find a tree. But what can trees lean upon for support?");
        }
    }
    
    if (randomQuoteInterval.checkDone() && !floroRandomQuotes.empty()) {
        std::string floroNextLine = floroRandomQuotes.front();
        floroRandomQuotes.pop();
         
        floroNextLines.push(floroNextLine);
         
        randomQuoteInterval.windGentle(45.0f);
        floroPauseSpeak.windGentle(4.0f);
    }
    
    if (floroPauseSpeak.checkDone() && !floroNextLines.empty()) {
        std::string floroNextLine = floroNextLines.front();
        floroNextLines.pop();
        
        float duration = floroNextLine.size() * 0.05f;
        speak("Floro", floroNextLine, duration); 
        
        if (!floroNextLines.empty()) { 
            floroPauseSpeak.windGentle(duration + 3.0f);
        }
    }

    if (floro->getDistanceTo(flowerPos) < 5.0f && !encounterFlower) {
        encounterFlower = true;
     //   floroNextLines.push("Why does the lonely flower blossom? There are no bees in the winter.");
      //  floroPauseSpeak.windGentle(4.0f);
    }
      
    if (floro->getDistanceTo(treePos) < 5.0f && !encounterTree) { 
        encounterTree = true;
        floroNextLines.push("In a grove of evergreen trees, this tree is an outlier. Does it not feel exiled?");
        floroPauseSpeak.windGentle(4.0f);
    }

    
    switch (step) {
        case 0: {
            if (!isWaiting()) {
      //      fadeToTextSequence(1.0f, "A thousand years ago, a Shorewalker genious wrote the first Theorem.", "The world was never the same after.", "Five years after that event,", "she left behind the society she established and disappeared into the Far North.", "No one knows exactly why.", "As I lie here in the cold snow,", "I can't help but think of that story.", "In the human world or the bleak Wilds, was she lonely wherever she went?");
                
                fadeToTextSequence(1.0f, "I won't let them take away my Elder Brother.", "Everything can be bought with money.", "I'll sell enough paintings to keep them away.", "If that doesn't work, I will steal him out of jail secretly.", "They are all wrong.");
  
                                   
                anim->playAnim("Lying", true); 
                  
                world->setDirectionalLight(midday);
                world->getDistanceFog().setDistanceFog(0.015, 1, glm::vec3(1.0,1.0,1.5));
                    
                PropFactory pf;
                auto grass = pf.makeProp(PROP_SNOW_FLOWER_BLUE);
                flowerPos = glm::vec3(12, 0, 28);
                flowerPos.y = world->getHeightAt(glm::vec2(flowerPos.x, flowerPos.z));
                grass->setPos(flowerPos);
                grass->addComponent<PaintingSubject>(*grass.get(), *grass.get(), *world, "Lonely Flower");
                world->insert(grass);
                 
                auto tree = pf.makeProp(PROP_TREE);
                treePos = getPos() + glm::vec3(-20,0,-30);
                treePos.y = world->getHeightAt(glm::vec2(treePos.x, treePos.z));
                tree->setPos(treePos);
                tree->addComponent<PaintingSubject>(*tree.get(), *tree.get(), *world, "Resilient Tree");
                world->insert(tree);

                world->getCameraRef().lock()->rotate(glm::vec3(60, 0, 0));
                
                floro->setPos(glm::vec3(18, 0, 33));
            }
            waitFor(34.0f);
            break; 
        } 
            
        case 1: {
            world->getComponent<NotificationSystem>()->newNotification("Art Subjects can be found around the world.", 3.0f);
            world->getComponent<NotificationSystem>()->newNotification("Look for floating sparkles.", 3.0f);
            step++;
            break;
        }
            
        case 2: {
            if (paintedSubjects.size() >= 1) {
                world->addComponent<DirlightTransition>(*world, 40.0f, sunset);
                world->addComponent<SkyColorTransition>(*world, 40.0f, glm::vec3(0.4, 0.18, 0.15));
                world->addComponent<FogTransition>(*world, 40.0f, 0.015, 1, glm::vec3(0.8, 0.4, 0.35));
                step++; 
            }
            break; 
        }
             
        case 3: {
            if (paintedSubjects.size() >= 2) {
                if (!isWaiting()) {
                    world->addComponent<DirlightTransition>(*world, 50.0f, midNight);
                    world->addComponent<SkyColorTransition>(*world, 50.0f, glm::vec3(0.04, 0.03, 0.05));
                    world->addComponent<FogTransition>(*world, 50.0f, 0.015, 1, glm::vec3(0.0, 0.0, 0.0)); 
                }
                waitFor(20.0f);
            }
            break;
        }
     
        case 4: {
            if (!isWaiting()) {
                floroNextLines = {};
                floroRandomQuotes = {};
                std::vector<std::string> lines = {"Wait...", "A lonely flower and a yearning tree. This place...", "Legend says in her young days, the First Mathmatician met a Moon spirit here, in the Windless Grove.", "She was never the same after that day.", "I don't see any moon spirits... do I?", "... but the moon in the sky is oh so beautiful tonight.", "I'll have to be a bit higher up to get a good view."};
                std::vector<float> durations = {3.0, 5.0f, 6.0f, 4.0f, 4.0f, 4.0f, 5.0f};
                makeSpeech("Floro", lines, durations);
            }
            waitFor(40.0f);
            break;
        }
            
        case 5: {
            if (floro->getPos().y > flowerPos.y + 10.0f) {
            if (!isWaiting()) {
            world->addComponent<DirlightTransition>(*world, 1.0f, midNight);
            world->addComponent<SkyColorTransition>(*world, 1.0f, glm::vec3(0.04, 0.03, 0.05));
            world->addComponent<FogTransition>(*world, 1.0f, 0.015, 1, glm::vec3(0.0, 0.0, 0.0));
             
                floroNextLines.push("If a Moon Spirit were to appear, perhaps it would right the wrongs Elder Brother is facing.");
                
                floroNextLines.push("But on my own, this painting... is very good.");
                
                floroNextLines.push("I'm hesitant to part with it, but it will sell.");
                
                
                floroPauseSpeak.windGentle(6.0f);
            } 
            waitFor(20.0f);
            }
            break;
        }
            
        case 6: {
            world->getComponent<NotificationSystem>()->newNotification("Head to the village.", 4.0f);
            endScene();
            break;
        }

    }
}
 

