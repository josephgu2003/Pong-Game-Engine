//
//  Moonlight.cpp
//  Pong
//
//  Created by Joseph Gu on 2/12/22.
//

#include "Moonlight.hpp"
#include "PropFactory.hpp"
#include "World.hpp"
#include "LifeTime.hpp"
#include "MovementController.hpp"
#include "SurvivalMode.hpp"

std::vector<std::string> lakeCrew = {
    "Floro", "Moonbell"
};

std::vector<std::string> lakeprereqs = {};
  
Moonlight::Moonlight(World* world, bool completed) : Script(world, lakeCrew, 30.0f, completed, "Moonlight", lakeprereqs) {
     
}

void Moonlight::act() {
    Actor* floro = getActorNamed("Floro");
    Actor* moonbell = getActorNamed("Moonbell");
    if (step < 8) moonbell->orientYawTo(floro);
    setPos(moonbell->getPos());
    
    auto cam = world->getCameraRef().lock();
    switch (step) {
            
        case 0: {
            if (!isWaiting()) {
            PropFactory pf;
            auto ray = pf.makeProp(PROP_AURORA);
            ray->setPos(glm::vec3(0, 70, 120));
            world->insert<Prop>(ray); 
            
            auto ray2 = pf.makeProp(PROP_AURORA);
            ray2->setPos(glm::vec3(30, 70, 90));
            world->insert<Prop>(ray2);
              
                world->getDistanceFog().setDistanceFog(0.03, 2, glm::vec3(0.023, 0.02, 0.03));
                DirectionalLight            dl2(glm::vec3(0.03,0.03,0.04),glm::vec3(0.05,0.05,0.06),glm::vec3(0.4,0.4,0.4),glm::vec3(-1,-1,0));
                world->setDirectionalLight(dl2);
                
                auto beam = pf.makeProp(PROP_LIGHTRAY); 
                beam->setPos(floro->getPos() + glm::vec3(0, 2, 0));
                beam->addComponent<LifeTime<Prop>>(*(beam.get()), 10.0f);
                beam->addComponent<LightComponent>(*beam.get(), *beam.get(), PointLight(glm::vec3(1.2,1.2,1.2), glm::vec3(3.5,3.5,3.5), glm::vec3(2.0,2.0,2.0), 1.0, 0.2, 0.2, glm::vec3(0)));
                world->insert(beam);
                cam->setState(CAM_FREE);
                cam->setPos(floro->getPos() + glm::vec3(0, 10, -30));
                cam->orientYawTo(floro);
            }
               
            waitFor(12.0f);
            break;
        }
            
        case 1: {
            if (!isWaiting()) {
                cam->setState(CAM_FOLLOW_ACTOR_ALIGNED);
                std::vector<std::string> lines = {"Oh! So you are okay!", "I really thought you wouldn't make it,", "but you have determination.", "How do you feel?"};
                std::vector<float> durations = {3.0f, 2.5f, 2.5f, 4.0f};
                 
                makeSpeech("Moonbell", lines, durations);
            }
            moonbell->jump();
            waitFor(12.0f);
            break;
        }
            
        case 2: {
            if (!isWaiting()) {
                speak("Floro", "I feel... strangely well.", 3.0f);
            }
            
            waitFor(5.0f);
            break;
        }
            
        case 3: {
            if (!isWaiting()) {
                std::vector<std::string> lines = {"Yes.", "I can't believe your guts,", "trying to help me despite being a layperson.", "You realize that even a hundred of you aren't worth as much as one me?", "But thank you.", "I didn't want you to die because of me,", "so I've returned the favor,", "at a cost."};
                std::vector<float> durations = {4.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f};
                
                makeSpeech("Moonbell", lines, durations);
            }
            
            waitFor(23.0f);
            break;
        }
            
        case 4: {
            if (!isWaiting()) {
                std::vector<std::string> lines = {"Returned the favor? At a cost?", "You don't mean..."};
                std::vector<float> durations = {5.0f, 5.0f};
                
                makeSpeech("Floro", lines, durations);
            }
            
            waitFor(12.0f);
            break;
        }
        case 5: {
            if (!isWaiting()) {
                
                std::vector<std::string> lines = {"Yes.", "I'm only mortal now.", "But you...", "You've inherited my power.", "I suppose I'll just have to stick with you forever :)", "I have given you my umbrella so you wouldn't mind letting me stay under it from time to time, no?"};
                std::vector<float> durations = {3.0f, 2.0f, 2.0f, 2.0f, 3.0f, 5.0f};
                 
                makeSpeech("Moonbell", lines, durations);
            }
            
            waitFor(19.0f);
            break;
        }
        case 6: {
            if (!isWaiting()) {
                std::vector<std::string> lines = {"I'm a Shorewalker now...?", "I helped you and you saved my life.", "If your powers are in me now, I can't leave you behind defenseless.", "So let's travel together until the world calms down!"};
                std::vector<float> durations = {5.0f, 3.0f, 3.0f, 5.0f};
                
                makeSpeech("Floro", lines, durations);
            } 
             
            waitFor(18.0f);
            break;
        }
             
        case 7: {
            if (!isWaiting()) {
                std::vector<std::string> lines = {"Okay!", "Let's get out of here first."};
                std::vector<float> durations = {5.0f, 5.0f};
                world->addComponent<SurvivalMode>(*world, 50.0f);
                makeSpeech("Moonbell", lines, durations);
            }
            moonbell->jump(); 
            waitFor(12.0f);
            break; 
        }
        case 8: {
            if (!isWaiting()) {
                moonbell->orientYawTo(glm::vec3(20, 0, -50) - moonbell->getPos());
            }
            waitFor(8.0f);
            break;
        }
        case 9: {
            std::vector<std::string> lines = {"You said your Teacher wanted you to find the incarnation of the moon?", "The night the moon disappeared, a ray of moonlight fell on the town behind that mountain.", "I heard there's a festival there too!"};
            std::vector<float> durations = {5.0f, 3.0f, 3.0f};
            
            makeSpeech("Moonbell", lines, durations);
            step++;
            break;
        }
        case 10: { 
            moonbell->getComponent<MovementController>()->move(MOVEDIR_FORWARDS);
            if (moonbell->getDistanceTo(glm::vec3(20, moonbell->getPos().y, -50)) < 5.0f) {
                endScene();
                step++;
            }
            break;
        } 
              
        default: 
            break;
    } 
}

// combat and progression is fun
// life skills are fun
// being a part of the story ?
