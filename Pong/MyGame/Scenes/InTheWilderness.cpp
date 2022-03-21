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
    
    floroRandomQuotes.push("Perhaps this journey is worth it. So many sights to see.");
    floroRandomQuotes.push("Teacher sent me into the world beneath the Mountain. It is a cold one.");
    
    randomQuoteInterval.windGentle(70.0f);
}

 
void InTheWilderness::act() {
    if (step < 4) step = 4; 
    Actor* floro = getActorNamed("Floro");
    AnimComponent* anim = floro->getComponent<AnimComponent>();
    Painter* painter = floro->getComponent<Painter>();
    std::string recentPainting = painter->getMostRecentPainting();
    
    if ((recentPainting != "") && (paintedSubjects.find(recentPainting) == paintedSubjects.end())) {
        paintedSubjects.insert(recentPainting);
        floroPauseSpeak.windGentle(4.0f);
         
        if (recentPainting == "Lonely Flower") {
            floroNextLines.push("Teacher would say this painting has no spirit. I'll rip it up then.");
        } else if (recentPainting == "Resilient Tree") {
            floroNextLines.push("Rip it up, rip it up! Am I so useless?");
        }
    }
    
    if (randomQuoteInterval.checkDone() && !floroRandomQuotes.empty()) {
        std::string floroNextLine = floroRandomQuotes.front();
        floroRandomQuotes.pop();
         
        floroNextLines.push(floroNextLine);
         
        randomQuoteInterval.windGentle(60.0f);
        floroPauseSpeak.windGentle(4.0f);
    } 
     
    
    if (floroPauseSpeak.checkDone() && !floroNextLines.empty()) {
        std::string floroNextLine = floroNextLines.front();
        floroNextLines.pop();
        
        speak("Floro", floroNextLine, 3.0f);
        
        if (!floroNextLines.empty()) {
            floroPauseSpeak.windGentle(6.0f);
        }
    }

    if (floro->getDistanceTo(flowerPos) < 5.0f && !encounterFlower) {
        encounterFlower = true;
        floroNextLines.push("To think that in the cold snow grows a beautiful flower...");
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
                fadeToTextSequence(1.0f, "Is it already noon?");
                anim->playAnim("Lying", true); 
                  
                world->setDirectionalLight(midday);
                world->getDistanceFog().setDistanceFog(0.015, 1, glm::vec3(1.0,1.0,1.5));
                    
                PropFactory pf;
                auto grass = pf.makeProp(PROP_SNOW_FLOWER_BLUE);
                flowerPos = getPos() + glm::vec3(0, 0, 20);
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
                  
                ParticleFactory parts;
                auto snow = parts.makeParticles(PE_SNOW, getPos() + glm::vec3(0, 10, 0));
                world->insert(snow);
            }
            waitFor(20.0f); 
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
                world->addComponent<DirlightTransition>(*world, 50.0f, sunset);
                world->addComponent<SkyColorTransition>(*world, 50.0f, glm::vec3(0.4, 0.18, 0.15));
                world->addComponent<FogTransition>(*world, 50.0f, 0.015, 1, glm::vec3(0.8, 0.4, 0.35));
                step++; 
            }
            break; 
        }
            
        case 3: {
            if (paintedSubjects.size() >= 2) {
                if (!isWaiting()) {
                    world->addComponent<DirlightTransition>(*world, 50.0f, earlyNight);
                    world->addComponent<SkyColorTransition>(*world, 50.0f, glm::vec3(0.12, 0.09, 0.12));
                    world->addComponent<FogTransition>(*world, 50.0f, 0.015, 1, glm::vec3(0.23, 0.2, 0.23));
                }
                waitFor(10.0f);
            }
            break;
        }
        case 4: {
            if (getActorNamed("Hue") == nullptr) {
                MyActorFactory maf;
                auto hue_ = maf.makeActor(ACTOR_SCARF_CHAR);
                hue_->addComponent<CombatComponent>(*hue_.get()); 
                newActor("Hue", hue_); 
                hue_->setPos(glm::vec3(0, 0, 0));
                hue_->orientYawTo(floro);
                
                auto oldman = maf.makeActor(ACTOR_SCARF_CHAR);
                oldman->addComponent<CombatComponent>(*oldman.get());
                newActor("Old Man", oldman);
                oldman->setPos(glm::vec3(100, 0, 100));
                oldman->orientYawTo(floro);
            }
            
            Actor* hueBoi = getActorNamed("Hue");
            
            if (hueBoi->getDistanceTo(floro) > 5.0f) {
                hueBoi->orientYawTo(floro);
                hueBoi->getComponent<MovementController>()->move(MOVEDIR_FORWARDS);
            } else {
                auto floroRef = getActorRefNamed("Floro");
                std::shared_ptr<Ability> letters = std::make_shared<FallingLetters>(&(hueBoi->getWorld()), hueBoi, 6.0f, floroRef); 
                hueBoi->getComponent<CombatComponent>()->newAbility(letters);
                step++;
            }
            break;
        }
        
        case 5: {
            if (!isWaiting()) {
                speak("Hue", "Speak, mortal, why have you come hither onto hallowed ground?", 4.0f);
            }
            waitFor(5.0f);
            break;
        }
        case 6: {
            if (!isWaiting()) {
                openChoiceMenu("Don't call me mortal as if you are a demigod.", "Shame on you for bullying the helpless.");
            }
            waitFor(100.0f);
            
            int option = getMenuChoiceAndClose();
            if (option == 0) {
                incStep(1, true);
            } else if (option == 1) {
                incStep(2, true); 
            } 
            break;
        } 
        case 7: { 
            if (!isWaiting()) {
                speak("Floro", "Don't call me mortal as if you are a demigod.", 4.0f);
            }
            waitFor(4.0f);
            break;
        }
        case 8: {
            if (!isWaiting()) {
                speak("Floro", "Shame on you for bullying the helpless.", 4.0f);
            }
            waitFor(4.0f);
            break;
        }
      /**  case 4: {
            if (!isWaiting()) {
            floroNextLines.push("Ah! This breeze! Why is the night so cold!");
            floroNextLines.push("A fire... In my bag...");
            floroPauseSpeak.windGentle(3.0f);
            }
            waitFor(10.0f);
           PropFactory pf;
            auto light = pf.makeProp(PROP_LIGHTRAY);
            light->setPos(glm::vec3(-70, 40, -60));
            world->insert(light);
            
            auto cage = pf.makeProp(PROP_IMPRISONMENT);
            cage->setPos(glm::vec3(-70, 40, -60));
            cage->setPosY(world->getHeightAt(glm::vec2(-70, -60)));
            cage->addComponent<LightComponent>(*cage.get(), *cage.get(), PointLight(glm::vec3(6.4,3.0,3.0), glm::vec3(18.5,13.0,13.0), glm::vec3(2.0,1.0,1.0), 1.0, 0.07, 0.008, glm::vec3(0)));
            world->insert(cage);
            break;
        }
            
        case 5: {
            world->getComponent<NotificationSystem>()->newNotification("Press F to light fire", 3.0f);
            step++;
            break;
        }
        
        case 5: {
            if (floro->getDistanceTo(glm::vec3(-70, floro->getPos().y, -60)) < 10.0f) {
                floro->getComponent<MovementController>()->paralyze(2.0f);
                anim->playAnim("Falling", false);
                floro->accelerate(0.12f * (-floro->getDir()+glm::vec3(0, 3.5, 0)));
                endScene();
            }
            break;
        }**/
    }
}
 

