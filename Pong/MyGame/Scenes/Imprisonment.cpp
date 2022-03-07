//
//  Imprisonment.cpp
//  Pong
//
//  Created by Joseph Gu on 3/4/22.
//

#include "Imprisonment.hpp"
#include "MyActorFactory.hpp"
#include "World.hpp"
#include "PropFactory.hpp"
#include "NameComponent.hpp"
#include "LifeComponent.hpp"
#include "AnimComponent.hpp"
#include "MovementController.hpp"

std::vector<std::string> imprisonmentCrew = {
    "Floro", "Moonbell"
};

std::vector<std::string> imprisonmentPrereqs = {"Moonlight"};
  
Imprisonment::Imprisonment(World* world, bool completed) : Script(world, imprisonmentCrew, 30.0f, completed, "Imprisonment", imprisonmentPrereqs) {
    wallCooldown = 0.0f;
}

void Imprisonment::act() {
    Actor* floro = getActorNamed("Floro");
    Actor* moonbell = getActorNamed("Moonbell");
    wallCooldown -= watch.getTime();
    watch.resetTime(); 
    if (step > 2) {
        floro->getComponent<LifeComponent>()->incStatValue(-0.05, STAT_LIFE);
    }
    
    if (auto i = imprisonment.lock()) {
        if (floro->getDistanceTo(i.get()) > 2.0f) {
            if (wallCooldown <= 0.0f) {
                floro->getComponent<AnimComponent>()->playAnim("Falling", 24, 100);
                floro->getComponent<MovementController>()->paralyze(3.0f);
                floro->accelerate(0.05f * (-floro->getDir()+glm::vec3(0, 1.5, 0)));
                watch.resetTime();
                wallCooldown = 1.0f; 
            }
        }
    }
    switch (step) {
        case 0: {
            if (!isWaiting()) {
                MyActorFactory maf;
                
                auto hue = maf.makeActor(ACTOR_SCARF_CHAR);
                hue->setPos(moonbell->getPos()+10.0f*moonbell->getDir());
                hue->orientYawTo(moonbell);

                auto huename = std::make_shared<NameComponent>(*hue.get());
                huename->init("Hue");
                hue->addComp(huename);
                 
                newActor("Hue", hue);
                
                PropFactory pf;
                auto prop = pf.makeProp(PROP_IMPRISONMENT);
                prop->setPos(moonbell->getPos());
                world->insert<Prop>(prop);
                speak("Moonbell", "*Gasps*", 2.0f);
                imprisonment = prop;
                floro->setPos(moonbell->getPos());
            }
               
            waitFor(5.0f);
            break;
        }
        case 1: {
            doAndWaitFor([&] () {speak("Floro", "We are so unlucky... this looks like a Geometric Imprisonment.", 3.0f);}, 5.0f);
            break;
        }  
              
        case 2: {
            if (!isWaiting()) {
                std::vector<std::string> lines = {"Indeed it is.", "The perfect alignment of mana flows to form the perfect prison.", "Beautiful isn't it? And deadly."};
                std::vector<float> durations = {5.0f, 3.0f, 3.0f};
                
                makeSpeech("Hue", lines, durations);
            } 
               
            waitFor(15.0f);
            break;
        }
             
        case 3: {
            if (!isWaiting()) {
                std::vector<std::string> lines = {"We can't get out,", "but he can't get us either.", "But if we stay here too long", "the damage inflicted by the Imprisonment will kill us.", "We have to break free...", "Oh... *coughs*", "My life is in your hands now.", "It seems like we are fated to look out for each other. :) *coughs*", "Don't fear.",  "Do what you do best. *gasp*", "Do what you do best...", "How else can you live your life? *closes eyes*"};
                std::vector<float> durations = {2.0f, 3.0f, 3.0f, 3.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 3.0f, 3.0f, 3.0f};
                makeSpeech("Moonbell", lines, durations);
            }
               
            waitFor(16.0f);
            break;
        }
        case 4: {
            moonbell->getComponent<AnimComponent>()->playAnim("Falling", 24, 100);
            moonbell->accelerate(0.1f * (-moonbell->getDir()+glm::vec3(0, 1.5, 0)));
            step++;
            break;
        } 
        case 5: {
            waitFor(25.0f);
            break;
        }
            
        case 6: {
            if (floro->getComponent<LifeComponent>()->getStat(STAT_LIFE).value <= 0.0f) {
                floro->getComponent<AnimComponent>()->playAnim("Falling", 24, 100);
                floro->getComponent<MovementController>()->paralyze(3.0f); 
                floro->accelerate(0.1f * (-floro->getDir()+glm::vec3(0, 1.5, 0)));
                step++;
            }
            break;
        }  
            
        default:
            break;
    }
}
 
 
