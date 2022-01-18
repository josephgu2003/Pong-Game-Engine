//
//  ScriptWakeUp.cpp
//  Pong
//
//  Created by Joseph Gu on 11/13/21.
//

#include "ScriptWakeUp.hpp"
#include "NameComponent.hpp"
#include "World.hpp"
#include "LifeComponent.hpp"
#include "PGraphicsComponent.hpp"
#include <algorithm>
#include "AnimComponent.hpp"
#include "NotificationSystem.hpp"
#include "FollowPos.hpp"
#include "InventoryComponent.hpp"

std::vector<std::string> myCrewWU = {
    "Floro", "Moonbell" 
};

std::vector<std::string> prereqsWU = {};
  
ScriptWakeUp::ScriptWakeUp(World* world, bool completed, std::weak_ptr<uiLayout> ui_) : Script(world, myCrewWU, 50.0f, completed, "FloroWakes", prereqsWU) {
    ui = ui_;
}
 
bool IntroPoem::tick(uiLayout* ui, float time) {
    auto setTransparency = [=] (float time_, Shader* s, float timeToStart, float timeToFade) {
        float alpha = glm::clamp(0.5f*(time_-firstTime-timeToStart), 0.0f, 1.0f);
        float alpha_ = glm::clamp(0.5f*(timeToFade-(time_-firstTime)),0.0f,1.0f);
        float realalpha = std::min(alpha, alpha_);
        s->use();
        s->setUniform("alpha", realalpha);
    };
    auto setTransparencyAll = [&] () {
        setTransparency(time, line1->getShader(), 2.0f,19.0f);
        setTransparency(time, line2->getShader(), 5.0f,19.0f);
        setTransparency(time, line3->getShader(), 8.0f,19.0f);
        setTransparency(time, line4->getShader(), 11.0f,19.0f);
        setTransparency(time, line5->getShader(), 14.0f,19.0f);
        setTransparency(time, uf->getShader(), -3.0f,22.0f);
    };
    
    switch (step) {
        case 0: {
            firstTime = time;
            uf = std::make_shared<uiFrame>(glm::vec2(-1,-1), glm::vec2(2,2), "Resources/GlyphsAndUI/blacksquare.png");
            line1 = std::make_shared<uiText>("Hopeful is the one ", -0.9, 0,DEFAULT_FONTSIZE, DEFAULT_LINESPACE);
            line2 = std::make_shared<uiText>("An elegy sung for I false hero.", -0.9, -0.1,DEFAULT_FONTSIZE, DEFAULT_LINESPACE);
            line3 = std::make_shared<uiText>("Every rising sun sets the evening before", -0.9, -0.2,DEFAULT_FONTSIZE, DEFAULT_LINESPACE);
            line4 = std::make_shared<uiText>("The black night, lit with stars no more.", -0.9, -0.3,DEFAULT_FONTSIZE, DEFAULT_LINESPACE);
            line5 = std::make_shared<uiText>("- The Hermit King", -0.9, -0.4, DEFAULT_FONTSIZE, DEFAULT_LINESPACE);
            uf->insertChild(line1);
            uf->insertChild(line2);
            uf->insertChild(line3);
            uf->insertChild(line4);
            uf->insertChild(line5); 
            ui->insertNode(uf);
            setTransparencyAll();
            step++;
             
            return false;
        }
             
        case 1: {
            if ((time - firstTime) > 22.0f) {
                ui->removeNode(uf.get());
                uf.reset();
                line1.reset();
                line2.reset();
                line3.reset();
                line4.reset();
                line5.reset();
                return true;
            } else { 
                setTransparencyAll();
            } 
            
            return false;
        }

    }

    return false;
}
void ScriptWakeUp::act() {
    setPos(getActorNamed("Moonbell")->getPos());
 
    if (glm::length(getActorNamed("Floro")->getPos() - getActorNamed("Moonbell")->getPos()) > 20.0) {
        if (stopWatch.getTime() > 4.0f) {
            world->getComponent<NotificationSystem>()->newNotification("You feel a need to follow this mysterious character", 3.0f);
            getActorNamed("Floro")->setPos(getActorNamed("Moonbell")->getPos()-5.0f*getActorNamed("Moonbell")->getDir());
        }
    }
        
    if (step > 3 && step < 9) {
        getActorNamed("Moonbell")->posDir(0.025);
    }
            
    switch (step) { 
        case 0: {
                snow = pf.makeParticles(PE_SNOW, getActorNamed("Moonbell")->getPos()+glm::vec3(0,2,0));
                snow->getComponent<GraphicsComponent>()->setColor(0.2, 0.2, 0.2);
            snow->addComponent<FollowPos<ParticleSystem>>(*(snow.get()), *(getActorNamed("Floro")), glm::vec3(0,2,0)); 
                world->insert<ParticleSystem>(snow);
                getActorNamed("Floro")->setPos(getPos()+glm::vec3(5,0,-5));
                getActorNamed("Floro")->getComponent<AnimComponent>()->playAnim("Lying", true);
                introPoem.step = 0;
                introPoem.tick(ui.lock().get(),stopWatch.getTime());
            if (auto c = camRef.lock()) {
                c->unlockFromActor(getActorNamed("Floro")->getPos()+glm::vec3(-0.5,10.0,0), glm::vec3(1,0,0));
            }
            step++;
            break;    
        }
        case 1: {
            if (introPoem.tick(ui.lock().get(),stopWatch.getTime())) step = 30;
            break;
        }
        case 30: {
            if (auto c = camRef.lock()) {
                c->translatePos(glm::vec3(0,-0.03,0));
                if (c->getPos().y - getActorNamed("Floro")->getPos().y < 0.5f) {
                    step = 2;
                    c->lockOntoActor();
                }
            }
            break;
        }
        case 2: {
            if (getActorNamed("Floro")->getDistanceTo(getActorNamed("Moonbell")) < 4.0f) {
                world->getComponent<NotificationSystem>()->newNotification("SNOW WANDERINGS", 5.0f);
                step++;
            }
            break; 
        }
        case 3: {
            if (!isWaiting()) {
            getActorNamed("Moonbell")->orientYawTo(glm::vec3(74,44,-2)-getActorNamed("Moonbell")->getPos());
            }
            waitFor(4.0f);
            break;
        }
        case 4: {
            if (!isWaiting()) {
                speak("Moonbell", "Come. Daylight is fading. It is time to move.", 3.0f);
            }
            waitFor(12.0f);
            break;
        }
        case 5: {
            if (!isWaiting()) {
                speak("Floro", "Where are we going?", 3.0f);
            }
            waitFor(6.5f);
            break;
        }
        case 6: {
            waitFor(26.0f);
            break;
        }

        case 7: {
            if (noActiveSpeech()) {
                std::vector<std::string> lines = {"You do not trust me yet.", "But at this moment of danger we both need each other."};
                std::vector<float> durations = {7.0f, 7.0f};
                makeSpeech("Moonbell", lines, durations);
                step++;
            }
            break;
        }
        case 8: {
            if(getActorNamed("Moonbell")->getDistanceTo(glm::vec3(74,getActorNamed("Moonbell")->getPos().y,-2)) < 0.5) {
                step++;
            } 
            break; 
        }
        case 9: {
            if (!isWaiting()) {
                getActorNamed("Moonbell")->getComponent<NameComponent>()->speak("Trust your instincts.", 2.0);
            }
            waitFor(3.0);
            break;
        }
        case 10: {
            world->deleteX<ParticleSystem>(snow.get());
            snow.reset(); 
            endScene();
            break; }
             
        default:
            break;
    }
}
 
 
// exposition points: lost memory,
 
