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
#include "DirlightTransition.hpp"

std::vector<std::string> inWildsCrew = {
    "Floro"
};

std::vector<std::string> inWildsReqs = {};

InTheWilderness::InTheWilderness(World* world, bool completed, InputHandler* ih, uiLayout* ui) : MyScript(world, inWildsCrew, 200.0f, completed, "In the Wilderness", inWildsReqs, ih, ui) {
    encounterGrass = false;
    encounterTree = false;
    
    floroRandomQuotes.push("Winter is white, black, but no other colors.");
    floroRandomQuotes.push("So many sights to see, but I can't paint them all.");
    floroRandomQuotes.push("Teacher sent me into the world beneath the Mountain. It is a cold one.");
    
    randomQuoteInterval.windGentle(70.0f);
}

 
void InTheWilderness::act() {
    Actor* floro = getActorNamed("Floro");
    AnimComponent* anim = floro->getComponent<AnimComponent>();
    Painter* painter = floro->getComponent<Painter>();
    std::string recentPainting = painter->getMostRecentPainting();
    
    if ((recentPainting != "") && (paintedSubjects.find(recentPainting) == paintedSubjects.end())) {
        paintedSubjects.insert(recentPainting);
        floroPauseSpeak.windGentle(4.0f);
         
        if (recentPainting == "Humble Grass") {
            floroNextLines.push("Teacher would say this painting has no spirit. I'll rip it up then.");
        } else if (recentPainting == "Resilient Tree") {
            floroNextLines.push("Rip it up, rip it up! Am I so useless?");
        }
    }
    
    if (randomQuoteInterval.checkDone() && !floroRandomQuotes.empty()) {
        std::string floroNextLine = floroRandomQuotes.front();
        floroRandomQuotes.pop();
         
        floroNextLines.push(floroNextLine);
        
        randomQuoteInterval.windGentle(40.0f);
        floroPauseSpeak.windGentle(4.0f);
    }
    
    if (floroPauseSpeak.checkDone() && !floroNextLines.empty()) {
        std::string floroNextLine = floroNextLines.front();
        floroNextLines.pop();
        
        speak("Floro", floroNextLine, 3.0f);
        
        if (!floroNextLines.empty()) {
            floroPauseSpeak.windGentle(4.0f);
        }
    }
     
    if (floro->getDistanceTo(grassPos) < 5.0f && !encounterGrass) {
        encounterGrass = true;
        floroNextLines.push("Graceful grass, swaying in the breeze.");
        floroPauseSpeak.windGentle(4.0f);
    }
     
    if (floro->getDistanceTo(treePos) < 5.0f && !encounterTree) {
        encounterTree = true;
        floroNextLines.push("Huh, this tree should be leafless by now.");
        floroPauseSpeak.windGentle(4.0f);
    } 
    // give a little intro text
    // "Floro, I'm not expelling you."
    // "I only hope a journey in the harsh world can bring life to your art."
    
    // Main char wakes up in the afternoon
    // "As if Teacher"
    
    // Try drawing 2 things:  
    
    // Tree
    // Rock
    // Grass 
    
    // Then gotta guide Floro onto the mountain peak
    
    // Try drawing fire
    
    // Try drawing moon
    
    // Moon disappears
    
    switch (step) {
        case 0: {
            if (!isWaiting()) {
                fadeToTextSequence(1.0f, "You should wake up. It's already noon.", "Don't you have many things to do?");
                anim->playAnim("Lying", true); 
                  
                world->setDirectionalLight(midday);
                world->getDistanceFog().setDistanceFog(0.015, 1, glm::vec3(1.0,1.0,1.5));
                  
                PropFactory pf;
                auto grass = pf.makeProp(PROP_GRASS);
                grassPos = getPos() + glm::vec3(10, 0, 10);
                grassPos.y = world->getHeightAt(glm::vec2(grassPos.x, grassPos.z));
                grass->setPos(grassPos);
                grass->addComponent<PaintingSubject>(*grass.get(), *grass.get(), *world, "Humble Grass");
                world->insert(grass);
                
                auto tree = pf.makeProp(PROP_TREE);
                treePos = getPos() + glm::vec3(-20,0,-30);
                treePos.y = world->getHeightAt(glm::vec2(treePos.x, treePos.z));
                tree->setPos(treePos);
                tree->addComponent<PaintingSubject>(*tree.get(), *tree.get(), *world, "Resilient Tree");
                world->insert(tree);
                  
                ParticleFactory parts;
                auto snow = parts.makeParticles(PE_SNOW, getPos() + glm::vec3(0, 10, 0));
                world->insert(snow);
            }
            waitFor(20.0f); 
            break; 
        } 
            
        case 1: {
            world->getComponent<NotificationSystem>()->newNotification("Explore and find inspirations for painting.", 4.0f);
            step++;
            break;
        } 
        case 2: {
            if (paintedSubjects.size() >= 2) {
                world->getComponent<NotificationSystem>()->newNotification("Well done.", 4.0f);
                step++; 
            }
            break; 
        }
        case 3: {
            if (!isWaiting()) {
                world->addComponent<DirlightTransition>(*world, 50.0f, sunset); 
                world->addComponent<SkyColorTransition>(*world, 50.0f, glm::vec3(0.5, 0.18, 0.15));
                world->addComponent<FogTransition>(*world, 50.0f, 0.01, 1, glm::vec3(0.8, 0.4, 0.35));
            }
            waitFor(50.0f);
            break;
        }
    }
}
 

